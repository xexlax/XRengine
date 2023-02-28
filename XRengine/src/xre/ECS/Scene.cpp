
#include "pch.h"
#include "Scene.h"

#include "xre\ECS\Components.h"
#include "xre\ECS\GameObject.h"
#include "xre\Renderer\Renderer3D.h"

namespace XRE{
	

	GameObject Scene::CreateGameObject(const std::string& Name)
	{
		GameObject go = { m_Registry.create(), this, Name };
		go.AddComponent<TransformComponent>();
		return go;
	}

	void Scene::OnUpdate(TimeStep ts)
	{
		Ref<Camera> mainCamera= nullptr;
		TransformComponent* cameraTransform = nullptr;
		{
			auto group = m_Registry.view<TransformComponent, CameraComponent>();
			for (auto entity : group)
			{
				auto& [transform, camera] = group.get<TransformComponent, CameraComponent>(entity);

				if (camera.m_Primary)
				{
					mainCamera = camera.m_Camera;
					cameraTransform = &transform;
					break;
				}
			}
		}

		auto group = m_Registry.group<TransformComponent>(entt::get<MeshRendererComponent>);

		Renderer3D::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		if (mainCamera&&cameraTransform)
		{
			mainCamera->SetPosition(cameraTransform->m_Translation);
			mainCamera->SetRotation(cameraTransform->m_Rotation);
			mainCamera->updateCameraVectors();

			Renderer3D::StartShadowPass();
			for (auto entity : group)
			{
				auto& [transform, meshrenderer] = group.get<TransformComponent, MeshRendererComponent>(entity);
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
					Renderer3D::DrawModel(meshrenderer, transform.GetTransform());
				}

				Renderer3D::DrawSkybox();
			}
			Renderer3D::EndScene();
			Renderer3D::m_FrameBuffer->Unbind();

		}
		
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


}