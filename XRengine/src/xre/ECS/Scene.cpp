
#include "pch.h"
#include "Scene.h"
#include "xre\Utils\address.h"

#include "xre\ECS\Components.h"
#include "xre\ECS\GameObject.h"
#include "xre\Renderer\Renderer3D.h"
#include "xre\ECS\NativeScript.h"
#include "xre\BluePrint\BluePrint.h"

#include "xre\Audio\alManager.h"




namespace XRE{
	/*void updatescript(Scene* sc) {
		
		auto &go = sc->GetObj(0).GetComponent<TransformComponent>();
		glm::vec3 g = go.GetGlobalEuler();
	

		float tx = g.z + g.y;
		float ty = g.x + g.z;

		float tz = g.x + g.y;

		go.SetGlobalRotation(glm::vec3(tx, ty, tz));

	}*/

	Scene::Scene()
	{
		m_PhysicsScene = PhysicsScene::Create();
		m_GlobalBluePrint = XMakeRef<BluePrint>();
		m_GlobalBluePrint->Global = true;
		m_GlobalProperties = XMakeRef<BluePrintProperties>();
		//Create Floor For Testing
		//m_PhysicsScene->Init();
	}
	GameObject Scene::CreateGameObject(const std::string& Name)
	{
		GameObject go = { m_Registry.create(), this};
		go.AddComponent<NameComponent>(Name);
		go.AddComponent<TransformComponent>();
		return go;
	}

	GameObject Scene::CreateFromFile(std::string fp)
	{
		ifstream fs(fp);
		cereal::JSONInputArchive ar(fs);

		GameObject go = CreateGameObject("1");
		ar(go);
		return go;
		
	}

	
	
	void Scene::OnUpdateRuntime(TimeStep ts)
	{
		
		//updatescript(this);

		//Physicss

		UpdatePhysics(ts);


		UpdateLogic(ts);
		UpdateAudio(ts);
		//UpdateNativeScripting(ts);
		
		XRef<Camera> mainCamera = nullptr;

		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);


			if (camera.m_Active) {

				camera.m_Camera->SetPosition(transform.m_Translation);
				camera.m_Camera->SetRotation(transform.m_Rotation);
				camera.m_Camera->updateCameraVectors();

				if (camera.m_Primary)
				{
					mainCamera = camera.m_Camera;
					break;
				}
			}


		}
		if(mainCamera)
		UpdateRendering(mainCamera);
		else {
			Renderer3D::SetClearColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
		}
		
	}

	void Scene::OnUpdateEditing(TimeStep ts, EditorCamera& ec)
	{
		
		
	
		

		//Rendering System
		XRef<Camera> mainCamera = make_shared<Camera>(ec);

		UpdateRendering(mainCamera);

		



	
		
	}

	std::pair<CameraComponent, TransformComponent> Scene::GetPrimaryCamera()
	{
		auto view = m_Registry.view<TransformComponent, CameraComponent>();
		for (auto entity : view)
		{
			auto [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);


			if (camera.m_Active) {

				camera.m_Camera->SetPosition(transform.m_Translation);
				camera.m_Camera->SetRotation(transform.m_Rotation);
				camera.m_Camera->updateCameraVectors();

				if (camera.m_Primary)
				{
					return {camera,transform };
					break;
				}
			}


		}
	}

	void Scene::Serialize(const std::string& filepath)
	{
		std::ofstream os(filepath);
		if (os.is_open()) {
			cereal::JSONOutputArchive archive(os);
			archive(*this);

			m_Registry.each([&](auto entityID)
				{
					//shared_ptr.get 返回其raw pointer
					GameObject go{ entityID , this };
					if(go.GetComponent<TransformComponent>().parent==nullptr)
					archive(go);
				});

			GameObject go = CreateGameObject("####TheEnder####");
			
			archive(go);


			Destroy(go);


		}
		
	}

	void Scene::Deserialize(const std::string& filepath)
	{
		m_FilePath = filepath;
		//m_Name = Utils::GetPureFileName(filepath);
		std::ifstream is(filepath);
		if (is.is_open()) {
			cereal::JSONInputArchive archive(is);
			archive(*this);

			GameObject go;

			while (true) {
				
				
				go = CreateGameObject("1");
				archive(go);
				if (go.GetComponent<NameComponent>().m_ObjName == "####TheEnder####") {
					Destroy(go);
					break;
				}
				
				

			}

			
			 
		}
		m_Name = Utils::GetPureFileName(filepath);
	}

	GameObject Scene::FindGOByPhysicBodyID(uint32_t pid)
	{
		auto view = m_Registry.view<RigidBodyComponent>();

		for (auto entity : view) {
			auto rbc = view.get<RigidBodyComponent>(entity);

			if (rbc.m_PhysicObj == pid) return GameObject(entity,this);

		}
	}

	vector<GameObject> Scene::FindGOByName(std::string str)
	{
		vector<GameObject> ans;
		auto view = m_Registry.view<NameComponent>();
			
		for (auto entity : view) {
			GameObject go(entity, this);
			if(go.GetName()==str)
			ans.push_back(go);
		}
		
		return ans;
	}

	GameObject Scene::GetObj(uint32_t eid)
	{
		return GameObject(entt::entity(eid), this);
	}

	

	void Scene::UpdateNativeScripting(TimeStep ts)
	{
		{
		m_Registry.view<NativeScriptComponent>().each(
			[=](entt::entity entity, NativeScriptComponent& nsc)
			{
				if (!nsc.Instance)
				{
					nsc.Instance = nsc.InstantiateScript();
					nsc.Instance->m_Object = GameObject{ entity, this };

					nsc.Instance->OnCreate();
				}

				nsc.Instance->OnUpdate(ts);
			});
		}


		{
			auto view = m_Registry.view<TransformComponent, AnimatorComponent>();
			for (auto entity : view) {
				auto& [transform, anm] = view.get<TransformComponent, AnimatorComponent>(entity);
				if (anm.m_Active) {
					anm.m_Phase += ts * anm.m_AngleSpeed;
					transform.m_Translation = anm.m_Pivot + anm.m_Radius * glm::vec3(cos(anm.m_Phase), 0, sin(anm.m_Phase));
				}
			}
		}
		{

			auto view = m_Registry.view<RigidBodyComponent, AnimatorComponent>();
			for (auto entity : view) {
				auto& [rbc, anm] = view.get<RigidBodyComponent, AnimatorComponent>(entity);
				if (anm.m_Active && rbc.m_MotionType == RigidBodyComponent::Kinematic) {
					rbc.m_Motion.linear_velocity = glm::cross(glm::vec3(0, 1, 0), glm::vec3(cos(anm.m_Phase), 0, sin(anm.m_Phase))) * anm.m_Radius;
				}
			}
		}
	}

	void Scene::UpdatePhysics(TimeStep ts)
	{
		{
			auto view = m_Registry.view<TransformComponent, RigidBodyComponent>();

			for (auto entity : view) {
				auto [tc, rbc] = view.get<TransformComponent, RigidBodyComponent>(entity);

				if (rbc.m_Active && rbc.m_MotionType != RigidBodyComponent::RigidBodyMotion::Trigger) {
					m_PhysicsScene->ResetRigidBody(tc, rbc);
				}

			}
		}

		m_PhysicsScene->OnUpdate(ts);


		{
			auto view = m_Registry.view<TransformComponent, RigidBodyComponent>();

			for (auto entity : view) {
				auto [tc, rbc] = view.get<TransformComponent, RigidBodyComponent>(entity);

				if (rbc.m_Active && rbc.m_MotionType != RigidBodyComponent::RigidBodyMotion::Trigger) {
					m_PhysicsScene->UpdateRigidBody(tc, rbc);
					
				}

			}
			for (auto entity : view) {
				auto [tc, rbc] = view.get<TransformComponent, RigidBodyComponent>(entity);

				if (rbc.m_Active) {
					
					m_PhysicsScene->UpdateCollision(tc, rbc);
					rbc.m_HitObjs.clear();
					for (auto x : rbc.m_HitResults) {
						if (x.m_HitBodyID != rbc.m_PhysicObj) {
							rbc.m_HitObjs.push_back(FindGOByPhysicBodyID(x.m_HitBodyID));
						}
					}
				}

			}
		}

		

		{
			auto view = m_Registry.view<TransformComponent,RayComponent>();

			for (auto entity : view) {
				auto [tc, rc] = view.get<TransformComponent, RayComponent>(entity);

				if (rc.m_Active){
					m_PhysicsScene->RayCast(tc.m_Translation, tc.GetGlobalDirection(), rc.m_MaxLength, rc.m_HitResults);
					
					//optional
					rc.m_HitObjs.clear();
					for (const PhysicsHitInfo& x : rc.m_HitResults) {
						rc.m_HitObjs.push_back(FindGOByPhysicBodyID(x.m_HitBodyID));
					}
					

				}

			}
		}

	}

	void Scene::UpdateAudio(TimeStep ts)
	{
		{
			auto view = m_Registry.view<TransformComponent,AudioListenerComponent>();
			
			for (auto entity : view) {
				auto [transform, al] = view.get<TransformComponent, AudioListenerComponent>(entity);
				if (al.m_Active) {
					alManager::SetListenerPos(transform.GetGlobalTranslation());
					GameObject go(entity, this);
					if (go.HasComponent<CameraComponent>()) {
						auto Cam = go.GetComponent<CameraComponent>().m_Camera;
						alManager::SetListenerOrientation(
							Cam->GetFront(), Cam->GetUp()
						);
					}
					break;
				}
			}
		}
		
		{
			auto view = m_Registry.view<TransformComponent, AudioSourceComponent>();

			for (auto entity : view) {
				auto [transform, as] = view.get<TransformComponent, AudioSourceComponent>(entity);
				if (as.m_Active) {
					for (auto it = as.auInstances.begin();it!=as.auInstances.end();) {
						auto& ai = *it;
						ai->SetPos(transform.GetGlobalTranslation());
						if (ai->TestEnded()) {
							it=as.auInstances.erase(it);
							
						}
						else it++;
					}
					
				}
			}
		}
	}

	void Scene::UpdateRendering(XRef<Camera> c)
	{
		bool dirLightFound = false;
		//Lighting
		{
			auto view = m_Registry.view<TransformComponent, DirectionalLightComponent>();

			for (auto entity : view) {
				auto [transform, dl] = view.get<TransformComponent, DirectionalLightComponent>(entity);
				if (dl.m_Active) {
					dirLightFound = true;
					Renderer3D::m_Light.SetDirLight(transform, dl);
					break;
				}
			}
		}

		{
			auto view = m_Registry.view<TransformComponent, PointLightComponent>();
			Renderer3D::m_Light.ClearPLights();
			for (auto entity : view) {
				auto [transform, l] = view.get<TransformComponent, PointLightComponent>(entity);
				if (l.m_Active) {
					Renderer3D::m_Light.AddPLight(transform, l);
				}
			}
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
		//auto dl = m_Registry.view<DirectionalLightComponent>();
		
		Renderer3D::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		if (c)
		{
			if (dirLightFound) {
				Renderer3D::StartShadowPass();
				for (auto entity : group)
				{
					auto& [transform, meshrenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);
					if (meshrenderer.m_Active && meshrenderer.m_ShadowCasting)
						Renderer3D::DrawMesh(meshrenderer.m_Model,transform.GetGlobalTransform());
				}
				Renderer3D::EndShadowPass();
			}



			// Coloring Pass
			Renderer3D::m_FrameBuffer->Bind();
			Renderer3D::Clear();
			Renderer3D::m_FrameBuffer->ClearAttachment(1, -1);

			//切shader一定要在startscene前
			Renderer3D::StartScene(c);
			{
				Renderer3D::activeShader->Bind();
				Renderer3D::activeShader->SetBool("Shadow_On", dirLightFound);
				Renderer3D::SetShadowMapOfActive(0);
				Renderer3D::DrawLight();

				for (auto obj : group)
				{
					auto& [transform, meshrenderer] = group.get<TransformComponent, MeshRendererComponent>(obj);
					if (meshrenderer.m_Active) {
						//Renderer3D::activeShader->Bind();
						Renderer3D::activeShader->SetInt("ObjID", int(obj));
						Renderer3D::DrawModel(meshrenderer, transform.GetGlobalTransform());
					}

				}

				{
					auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();

					for (auto entity : view) {
						auto [transform, spr] = view.get<TransformComponent, SpriteRendererComponent>(entity);
						if (spr.m_Active&&spr.m_Sprite) {
							Renderer3D::activeShader->SetInt("ObjID", int(entity));
							
							Renderer3D::DrawSprite(spr, transform.GetGlobalTransform());
						}
					}
				}
				if (m_Runtime == false)
				{
					auto view = m_Registry.view<TransformComponent, RigidBodyComponent>();


					for (auto entity : view) {
						auto [tc, rbc] = view.get<TransformComponent, RigidBodyComponent>(entity);

						if (rbc.m_Active) {
							if (rbc.m_ShowShape) {


								glm::vec4 color;
								switch (rbc.m_MotionType)
								{
								case RigidBodyComponent::Dynamic:
									color = glm::vec4(0, 1, 0, 1);
									break;
								case RigidBodyComponent::Kinematic:
									color = glm::vec4(0, 0.75, 0.75, 1);
									break;
								case RigidBodyComponent::Static:
									color = glm::vec4(0, 0, 1, 1);
									break;
								case RigidBodyComponent::Trigger:
									color = glm::vec4(0.7f, 1, 0, 1);
									break;

								default:
									break;
								}

								Renderer3D::DrawShapeFrame(rbc.m_Shape, tc.GetGlobalTransform(),color);
							}
						}

					}

				}

				{
					auto view = m_Registry.view<TransformComponent, RayComponent>();


					for (auto entity : view) {
						auto [tc, rc] = view.get<TransformComponent, RayComponent>(entity);

						if (rc.m_Active) {
							if (rc.m_Display) {
								Renderer3D::DrawRay(tc.GetGlobalTranslation(),tc.GetGlobalDirection(),rc.m_MaxLength);
							}
						}

					}

				}
				

				Renderer3D::DrawSkybox();
			}
			Renderer3D::EndScene();
			Renderer3D::m_FrameBuffer->Unbind();
		}
	}

	void Scene::UpdateLogic(TimeStep ts)
	{
		auto view = m_Registry.view<BluePrintComponent>();

		m_GlobalBluePrint->OnUpdate(entt::null, this, ts, m_GlobalProperties);
		for (auto entity : view) {
			auto bpc = view.get<BluePrintComponent>(entity);

			if (bpc.m_Active) {
				
				bpc.m_BluePrint->OnUpdate(entity, this ,ts, bpc.m_BluePrintProperties);
			}

		}
	}

	

	void Scene::Destroy(GameObject go)
	{
		if (m_Runtime == true) {
			if (go.HasComponent<RigidBodyComponent>()) {
				auto& rbc = go.GetComponent<RigidBodyComponent>();
				if(rbc.m_MotionType != RigidBodyComponent::Trigger)
				m_PhysicsScene->RemoveRigidBody(rbc.m_PhysicObj);
			}
			
		}
		m_Registry.destroy(go);

		
	}

	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		// Resize our non-FixedAspectRatio cameras
		auto view = m_Registry.view<CameraComponent>();
		for (auto entity : view)
		{
			auto& cameraComponent = view.get<CameraComponent>(entity);
			if (!cameraComponent.m_FixedAspectRatio)
				cameraComponent.m_Camera->SetViewPortSize(width, height);
		}

	}

	void Scene::OnRuntimeBegin()
	{
		m_Runtime = true;
		//this->Save();
		
		//Logic
		{
			auto view = m_Registry.view<BluePrintComponent>();

			m_GlobalBluePrint->OnRuntimeBegin();
			m_GlobalProperties->Update(m_GlobalBluePrint->GetDefaultProperties());
			for (auto entity : view) {
				auto bpc = view.get<BluePrintComponent>(entity);

				if (bpc.m_Active) {

					bpc.m_BluePrint->OnRuntimeBegin();
					bpc.m_BluePrintProperties->Update(bpc.m_BluePrint->GetDefaultProperties());
				}

			}
		}
		//Physics
		{
			auto view = m_Registry.view<TransformComponent, RigidBodyComponent>();

			for (auto entity : view) {
				auto [tc, rbc] = view.get<TransformComponent, RigidBodyComponent>(entity);

				if (rbc.m_Active && rbc.m_MotionType!= RigidBodyComponent::RigidBodyMotion::Trigger) {
					
					rbc.m_PhysicObj = m_PhysicsScene->CreateRigidBody(tc, rbc);
					rbc.m_PhysicsMaterial.changed = false;
				}

			}
		}
		//Rendering
		{
			
			auto view = m_Registry.view<CameraComponent>();
			for (auto entity : view) {
				auto cc = view.get<CameraComponent>(entity);

				cc.m_Camera->SetViewPortSize(m_ViewportWidth, m_ViewportHeight);
			
			}
		}
		//Audio
		{
			auto view = m_Registry.view<TransformComponent, AudioSourceComponent>();

			for (auto entity : view) {
				auto [transform, as] = view.get<TransformComponent, AudioSourceComponent>(entity);
				as.auInstances.clear();
			}
		}
		alManager::SetListenerPos(glm::vec3(100.0f, 0, 0));


		
	}

	void Scene::OnRuntimeEnd()
	{

		{
			auto view = m_Registry.view<TransformComponent, RigidBodyComponent>();

			for (auto entity : view) {
				auto [tc, rbc] = view.get<TransformComponent, RigidBodyComponent>(entity);

				if (rbc.m_Active && rbc.m_MotionType != RigidBodyComponent::RigidBodyMotion::Trigger) {
					m_PhysicsScene->RemoveRigidBody(rbc.m_PhysicObj);
					rbc.m_HitResults.clear();
				}

			}
		}
		alManager::StopAll();

		{
			auto view = m_Registry.view<TransformComponent, AudioSourceComponent>();

			for (auto entity : view) {
				auto [transform, as] = view.get<TransformComponent, AudioSourceComponent>(entity);
				as.auInstances.clear();
			}
		}


		m_Runtime = false;
		//Load();
	}


	

	template<typename T>
	void Scene::OnComponentAdded(GameObject go, T& component)
	{
		//static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<TransformComponent>(GameObject go, TransformComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<CameraComponent>(GameObject go, CameraComponent& component)
	{
		component.m_Camera->SetViewPortSize(m_ViewportWidth, m_ViewportHeight);
	}

	template<>
	void Scene::OnComponentAdded<MeshRendererComponent>(GameObject go, MeshRendererComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NameComponent>(GameObject go, NameComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<NativeScriptComponent>(GameObject go, NativeScriptComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<PointLightComponent>(GameObject go, PointLightComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<DirectionalLightComponent>(GameObject go, DirectionalLightComponent& component)
	{
	}

	template<>
	void Scene::OnComponentAdded<RigidBodyComponent>(GameObject go, RigidBodyComponent& component)
	{
		
	}

	template<>
	void Scene::OnComponentAdded<RayComponent>(GameObject go, RayComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<BluePrintComponent>(GameObject go, BluePrintComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<AudioSourceComponent>(GameObject go, AudioSourceComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<AudioListenerComponent>(GameObject go, AudioListenerComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<SpriteRendererComponent>(GameObject go, SpriteRendererComponent& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<XRPlayerComponent>(GameObject go, XRPlayerComponent& component)
	{

	}
}