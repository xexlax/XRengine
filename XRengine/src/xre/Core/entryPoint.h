#pragma once


extern XRE::Application* XRE::CreateApplication();

int main(int argc, char** argv) {
	XRE::Log::Init();
	XRE_CORE_WARN("XRE 已启动");
	XRE_INFO("你好，世界");



	auto app = XRE::CreateApplication();
	app->Run();
	delete app;
}

