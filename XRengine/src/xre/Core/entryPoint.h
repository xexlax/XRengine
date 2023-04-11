#pragma once


extern XRE::Application* XRE::CreateApplication();

int main(int argc, char** argv) {
	XRE::Log::Init();
	
	//XRE_INFO("ÄãºÃ£¬ÊÀ½ç");



	auto app = XRE::CreateApplication();
	app->Run();
	delete app;
}

