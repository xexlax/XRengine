#pragma once


extern XRE::Application* XRE::CreateApplication();

int main(int argc, char** argv) {
	XRE::Log::Init();
	
	//XRE_INFO("��ã�����");



	auto app = XRE::CreateApplication();
	app->Run();
	delete app;
}

