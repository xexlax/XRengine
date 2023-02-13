#pragma once

extern XRE::Application* XRE::CreateApplication();

int main(int argc, char** argv) {
	auto app = XRE::CreateApplication();
	app->Run();
	delete app;
}
