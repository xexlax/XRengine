#pragma once

namespace XRE {

	class PhysicsAPI
	{
	public:
		enum class API
		{
			None = 0, Jolt = 1
		};
	public:

		inline static API GetAPI() { return s_API; }
	private:
		static API s_API;
	};

	PhysicsAPI::API PhysicsAPI::s_API = PhysicsAPI::API::Jolt;
}