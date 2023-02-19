#pragma once


extern XRE::Application* XRE::CreateApplication();

int main(int argc, char** argv) {
	XRE::Log::Init();
	XRE_CORE_WARN("XRE");
	int var = 9;
	XRE_INFO("hello,{0}", var);



	auto app = XRE::CreateApplication();
	app->Run();
	delete app;
}

