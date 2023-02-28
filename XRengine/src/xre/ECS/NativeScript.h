#include "Components.h"
#include "GameObject.h"

namespace XRE {

	class ScriptableGameObject
	{
	public:
		template<typename T>
		T& GetComponent()
		{
			return m_Object.GetComponent<T>();
		}
	protected:
		virtual void OnCreate() {}
		virtual void OnDestroy() {}
		virtual void OnUpdate(TimeStep ts) {}
	private:
		GameObject m_Object;
		friend class Scene;
	};



	struct NativeScriptComponent : public Component
	{

		ScriptableGameObject* Instance = nullptr;

		
		ScriptableGameObject* (*InstantiateScript)();
		void (*DestroyScript)(NativeScriptComponent*);

		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableGameObject*>(new T()); };
			DestroyScript = [](NativeScriptComponent* nsc) { delete nsc->Instance; nsc->Instance = nullptr; };
		}
	};
}
