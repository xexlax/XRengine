
#include "pch.h"
#include "Scene.h"

#include "xre\ECS\Components.h"
#include "xre\ECS\GameObject.h"
#include "xre\Renderer\Renderer3D.h"
#include "xre\ECS\NativeScript.h"




namespace XRE{
	

	GameObject Scene::CreateGameObject(const std::string& Name)
	{
		GameObject go = { m_Registry.create(), this};
		go.AddComponent<NameComponent>(Name);
		go.AddComponent<TransformComponent>();
		return go;
	}

	
	
	void Scene::OnUpdate(TimeStep ts)
	{
		//Native Scripting
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

		//Lighting
		{
			auto view = m_Registry.view<TransformComponent, DirectionalLightComponent>();

			for (auto entity : view) {
				auto [transform,dl] = view.get<TransformComponent, DirectionalLightComponent>(entity);
				if (dl.m_Active) {
					Renderer3D::m_Light.SetDirLight(transform,dl);
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

		//Rendering System
		Ref<Camera> mainCamera= nullptr;
		TransformComponent* cameraTransform=nullptr;
		auto cct = GetPrimaryCamera(); 
		mainCamera = cct.first.m_Camera;
		cameraTransform = &cct.second;
		auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
		//auto dl = m_Registry.view<DirectionalLightComponent>();

		Renderer3D::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		if (mainCamera&&cameraTransform)
		{
	
			Renderer3D::StartShadowPass();
			for (auto entity : group)
			{
				auto& [transform, meshrenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);
				if(meshrenderer.m_Active && meshrenderer.m_ShadowCasting)
				Renderer3D::DrawModel(meshrenderer, transform.GetTransform());
			}
			Renderer3D::EndShadowPass();


			// Coloring Pass
			Renderer3D::m_FrameBuffer->Bind();
			Renderer3D::Clear();
			//切shader一定要在startscene前
			Renderer3D::StartScene(mainCamera);
			{
				Renderer3D::activeShader->Bind();
				Renderer3D::SetShadowMapOfActive(0);
				Renderer3D::DrawLight();

				for (auto entity : group)
				{
					auto& [transform, meshrenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);
					if(meshrenderer.m_Active)
					Renderer3D::DrawModel(meshrenderer, transform.GetTransform());
				}

				//Renderer3D::DrawSkybox();
			}
			Renderer3D::EndScene();
			Renderer3D::m_FrameBuffer->Unbind();

		}
		
	}

	void Scene::OnUpdateEditing(TimeStep ts, EditorCamera& ec)
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

		//Easy Animator
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

		//Rendering System
		Ref<Camera> mainCamera = make_shared<Camera>(ec);
	
		auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);
		//auto dl = m_Registry.view<DirectionalLightComponent>();

		Renderer3D::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		if (mainCamera)
		{
			if (dirLightFound) {
				Renderer3D::StartShadowPass();
				for (auto entity : group)
				{
					auto& [transform, meshrenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);
					if (meshrenderer.m_Active && meshrenderer.m_ShadowCasting)
						Renderer3D::DrawModel(meshrenderer, transform.GetTransform());
				}
				Renderer3D::EndShadowPass();
			}
			


			// Coloring Pass
			Renderer3D::m_FrameBuffer->Bind();
			Renderer3D::Clear();
			Renderer3D::m_FrameBuffer->ClearAttachment(1, -1);

			//切shader一定要在startscene前
			Renderer3D::StartScene(mainCamera);
			{
				Renderer3D::activeShader->Bind();
				Renderer3D::activeShader->SetBool("Shadow_On",dirLightFound);
				Renderer3D::SetShadowMapOfActive(0);
				Renderer3D::DrawLight();

				for (auto obj : group)
				{
					auto& [transform, meshrenderer] = group.get<TransformComponent, MeshRendererComponent>(obj);
					if (meshrenderer.m_Active) {
						//Renderer3D::activeShader->Bind();
						Renderer3D::activeShader->SetInt("ObjID", int(obj));
						Renderer3D::DrawModel(meshrenderer, transform.GetTransform());
					}
						
				}

				Renderer3D::DrawSkybox();
			}
			Renderer3D::EndScene();
			Renderer3D::m_FrameBuffer->Unbind();
		}
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
	}

	

	void Scene::Destroy(GameObject go)
	{
		
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


}