
// #pragma warning(disable : 26812) //The enum type 'XrResult' is unscoped.Prefer 'enum class' over 'enum' (Enum.3)
#pragma once

#include <iostream>

#include <stdio.h>
#include <vector>
//#define XR_NO_PROTOTYPES 1


#include "openxr/openxr.h"
//#define XR_USE_GRAPHICS_API_VULKAN 1

#define XR_USE_GRAPHICS_API_OPENGL

#define XR_USE_PLATFORM_WIN32 1


#include <Windows.h>
#include "openxr/openxr_platform.h"


//#include <GL/glew.h>
// note: replacing glew with glad
#include <Glad\glad.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32 1
#define GLFW_EXPOSE_NATIVE_WGL 1
#include <GLFW/glfw3native.h>

//#include "GLSLShader.h"

#include <XRE.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
/*PFN_xrGetInstanceProcAddr xrGetInstanceProcAddr;
PFN_xrEnumerateApiLayerProperties xrEnumerateApiLayerProperties;
PFN_xrEnumerateInstanceExtensionProperties xrEnumerateInstanceExtensionProperties;
PFN_xrCreateInstance xrCreateInstance;
PFN_xrDestroyInstance xrDestroyInstance;
PFN_xrPollEvent xrPollEvent;
*/
using namespace XRE;

PFN_xrCreateDebugUtilsMessengerEXT xrCreateDebugUtilsMessengerEXTd;
PFN_xrDestroyDebugUtilsMessengerEXT xrDestroyDebugUtilsMessengerEXTd;
PFN_xrGetOpenGLGraphicsRequirementsKHR xrGetOpenGLGraphicsRequirementsKHRd;

const char* ViewConfigTypeToStr(XrViewConfigurationType t)
{
	switch (t) {
	case XR_VIEW_CONFIGURATION_TYPE_PRIMARY_MONO:
		return "PRIMARY_MONO";
	case XR_VIEW_CONFIGURATION_TYPE_PRIMARY_STEREO:
		return "PRIMARY_STEREO";
	case XR_VIEW_CONFIGURATION_TYPE_PRIMARY_QUAD_VARJO:
		return "PRIMARY_QUAD";
	case XR_VIEW_CONFIGURATION_TYPE_SECONDARY_MONO_FIRST_PERSON_OBSERVER_MSFT:
		return "SECONDARY_MONO_FIRST_PERSON_OBSERVER";
	}
	return "Unknown";
}

XrBool32 DebugMessengerCallback(XrDebugUtilsMessageSeverityFlagsEXT messageSeverity, XrDebugUtilsMessageTypeFlagsEXT messageTypes, const XrDebugUtilsMessengerCallbackDataEXT* callbackData, void* userData)
{
	printf("%s %s %s\n", callbackData->messageId, callbackData->functionName, callbackData->message);
	return XR_FALSE;
}

float cubeVerts[]{
	-1,-1,-1,
	1,-1,-1,
	-1,1,-1,
	1,1,-1,
	-1,-1,1,
	1,-1,1,
	-1,1,1,
	1,1,1
};

uint16_t cubeInds[]{
	0,1,
	0,2,
	2,3,
	1,3,

	4,5,
	4,6,
	6,7,
	5,7,

	0,4,
	1,5,
	2,6,
	3,7
};

glm::mat4 FrustumXR(XrFovf fov, float nearf, float farf)
{
	return glm::frustum(nearf * tan(fov.angleLeft), nearf * tan(fov.angleRight), nearf * tan(fov.angleDown), nearf * tan(fov.angleUp), nearf, farf);
}

void CheckXrResult(XrResult r, const char* func)
{
	//if (XR_FAILED(r)) 
	if (r != XR_SUCCESS)
	{
		char buffer[XR_MAX_RESULT_STRING_SIZE] = "";
		xrResultToString(XR_NULL_HANDLE, r, buffer);
		printf("%s failed %s\n", func, buffer);
	}
}



int main(int carc, const char** argv)
{
	printf("Starting\n");

	HDC dc{};
	HGLRC glrc{};

	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	GLFWwindow* window = glfwCreateWindow(800, 480, "OpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// replaced glew with glad
	GLenum glew_status = gladLoadGL();

	/* or Check if GLAD is ok after valid context */
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" <<std::endl;
		return -1;
	}

	/*
	GLenum glew_status = glewInit();
	if (glew_status != GLEW_OK)
	{
		printf("Error: %s\n", glewGetErrorString(glew_status));
		return EXIT_FAILURE;
	}
	*/

	dc = GetDC(glfwGetWin32Window(window));
	//dc = GetDC(NULL);
	glrc = glfwGetWGLContext(window);
	printf("gwfw dc %p glrc %p\n", dc, glrc);
	

	XRef<Shader> XREshader = Shader::Create("..\\Assets\\shaders\\simple.glsl");

	//init
	XrResult r = XR_SUCCESS;
	/*	r = xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrEnumerateApiLayerProperties", (PFN_xrVoidFunction*)&xrEnumerateApiLayerProperties);
		r = xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrEnumerateInstanceExtensionProperties", (PFN_xrVoidFunction*)&xrEnumerateInstanceExtensionProperties);
		r = xrGetInstanceProcAddr(XR_NULL_HANDLE, "xrCreateInstance", (PFN_xrVoidFunction*)&xrCreateInstance);

		printf("xrEnumerateApiLayerProperties %p\n", xrEnumerateApiLayerProperties);
		printf("xrEnumerateInstanceExtensionProperties %p\n", xrEnumerateInstanceExtensionProperties);
		printf("xrCreateInstance %p\n", xrCreateInstance);
	*/

	uint32_t layerCount = 0;
	xrEnumerateApiLayerProperties(0, &layerCount, nullptr);
	printf("API layers count %d\n", layerCount);
	if (layerCount) {
		std::vector<XrApiLayerProperties> layers(layerCount, { XR_TYPE_API_LAYER_PROPERTIES,nullptr });
		xrEnumerateApiLayerProperties(layerCount, &layerCount, layers.data());
		for (const auto& l : layers) {
			printf("  %s: %s\n", l.layerName, l.description);
		}
	}
	uint32_t extCount = 0;
	xrEnumerateInstanceExtensionProperties(nullptr, 0, &extCount, nullptr);
	printf("Extensions count %d\n", extCount);
	if (extCount) {
		std::vector<XrExtensionProperties> exts(extCount, { XR_TYPE_EXTENSION_PROPERTIES,nullptr });
		xrEnumerateInstanceExtensionProperties(nullptr, extCount, &extCount, exts.data());
		for (const auto& e : exts) {
			printf("  %s\n", e.extensionName);
		}
	}

	

	

	XrDebugUtilsMessengerEXT debugMessenger = XR_NULL_HANDLE;
	
	
	XrInstance instance = XR_NULL_HANDLE;
	std::vector <const char*> enabledExts{ XR_EXT_DEBUG_UTILS_EXTENSION_NAME, XR_KHR_OPENGL_ENABLE_EXTENSION_NAME };
	XrInstanceCreateInfo instInfo{ XR_TYPE_INSTANCE_CREATE_INFO, nullptr, 0, {"xr-test",1,"",0,XR_MAKE_VERSION(1,0,0) }, 0,nullptr, (uint32_t)enabledExts.size(),enabledExts.data() };
	r = xrCreateInstance(&instInfo, &instance);
	if (XR_FAILED(r)) {
		char buffer[XR_MAX_RESULT_STRING_SIZE] = "";
		xrResultToString(XR_NULL_HANDLE, r, buffer);
		printf("xrCreateInstance failed %s\n", buffer);
		return -1;
	}
	printf("OpenXR instance created %p\n", instance);

	//r = xrGetInstanceProcAddr(instance, "xrDestroyInstance", (PFN_xrVoidFunction*)&xrDestroyInstance);
	//r = xrGetInstanceProcAddr(instance, "xrPollEvent", (PFN_xrVoidFunction*)&xrPollEvent);
	r = xrGetInstanceProcAddr(instance, "xrCreateDebugUtilsMessengerEXT", (PFN_xrVoidFunction*)&xrCreateDebugUtilsMessengerEXTd);
	r = xrGetInstanceProcAddr(instance, "xrDestroyDebugUtilsMessengerEXT", (PFN_xrVoidFunction*)&xrDestroyDebugUtilsMessengerEXTd);
	r = xrGetInstanceProcAddr(instance, "xrGetOpenGLGraphicsRequirementsKHR", (PFN_xrVoidFunction*)&xrGetOpenGLGraphicsRequirementsKHRd);


	XrDebugUtilsMessengerCreateInfoEXT messengerInfo{ XR_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT, nullptr,
		XR_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		XR_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | XR_DEBUG_UTILS_MESSAGE_TYPE_CONFORMANCE_BIT_EXT,
		DebugMessengerCallback,nullptr };
	r = xrCreateDebugUtilsMessengerEXTd(instance, &messengerInfo, &debugMessenger);

	XrInstanceProperties instProps{ XR_TYPE_INSTANCE_PROPERTIES,nullptr };
	r = xrGetInstanceProperties(instance, &instProps);
	printf("Runtime: %s: %d.%d.%d\n", instProps.runtimeName, XR_VERSION_MAJOR(instProps.runtimeVersion), XR_VERSION_MINOR(instProps.runtimeVersion), XR_VERSION_PATCH(instProps.runtimeVersion));

	XrSystemId systemId = XR_NULL_SYSTEM_ID;
	XrFormFactor formFactor = XR_FORM_FACTOR_HEAD_MOUNTED_DISPLAY;
	XrSystemGetInfo sysGetInfo{ XR_TYPE_SYSTEM_GET_INFO, nullptr, formFactor };
	r = xrGetSystem(instance, &sysGetInfo, &systemId);
	if (XR_FAILED(r)) {
		char buffer[XR_MAX_RESULT_STRING_SIZE] = "";
		xrResultToString(XR_NULL_HANDLE, r, buffer);
		printf("xrGetSystem failed %s\n", buffer);
		xrDestroyInstance(instance);
		return -1;
	}
	printf("Got HMD system id: %llX\n", systemId);

	XrSystemProperties sysProps{ XR_TYPE_SYSTEM_PROPERTIES,nullptr };
	r = xrGetSystemProperties(instance, systemId, &sysProps);
	printf("System: %s, vendor %d, max resolution %dx%d, tracking: orientation %d position %d\n", sysProps.systemName, sysProps.vendorId,
		sysProps.graphicsProperties.maxSwapchainImageWidth, sysProps.graphicsProperties.maxSwapchainImageHeight,
		sysProps.trackingProperties.orientationTracking, sysProps.trackingProperties.positionTracking);

	XrViewConfigurationType viewConfigType{};

	uint32_t viewConfigCount = 0;
	r = xrEnumerateViewConfigurations(instance, systemId, 0, &viewConfigCount, nullptr);
	if (viewConfigCount) {
		printf("View configurations: %d\n", viewConfigCount);
		std::vector<XrViewConfigurationType> viewConfigs(viewConfigCount);
		r = xrEnumerateViewConfigurations(instance, systemId, (uint32_t)viewConfigs.size(), &viewConfigCount, viewConfigs.data());
		XrViewConfigurationProperties viewConfigProps{ XR_TYPE_VIEW_CONFIGURATION_PROPERTIES,nullptr };
		for (const auto& vc : viewConfigs) {
			printf("  %s (%d)", ViewConfigTypeToStr(vc), vc);
			r = xrGetViewConfigurationProperties(instance, systemId, vc, &viewConfigProps);
			printf(" fov mutable %d\n", viewConfigProps.fovMutable);
		}
		viewConfigType = viewConfigs[0];
	}

	uint32_t cfgViewCount = 0;
	std::vector<XrViewConfigurationView> cfgViews;
	r = xrEnumerateViewConfigurationViews(instance, systemId, viewConfigType, 0, &cfgViewCount, nullptr);
	if (viewConfigCount) {
		printf("Config views: %d\n", cfgViewCount);
		cfgViews.resize(cfgViewCount, { XR_TYPE_VIEW_CONFIGURATION_VIEW, nullptr });
		r = xrEnumerateViewConfigurationViews(instance, systemId, viewConfigType, (uint32_t)cfgViews.size(), &cfgViewCount, cfgViews.data());
		for (const auto& v : cfgViews) {
			printf("  recomended %dx%dx%d, max %dx%dx%d\n", v.recommendedImageRectWidth, v.recommendedImageRectHeight, v.recommendedSwapchainSampleCount, v.maxImageRectWidth, v.maxImageRectHeight, v.maxSwapchainSampleCount);
		}
	}

	uint32_t envBlendModesCount = 0;
	r = xrEnumerateEnvironmentBlendModes(instance, systemId, viewConfigType, 0, &envBlendModesCount, nullptr);
	if (envBlendModesCount) {
		printf("EnvironmentBlendModes: %d\n", envBlendModesCount);
		std::vector<XrEnvironmentBlendMode> blendModes(envBlendModesCount);
		r = xrEnumerateEnvironmentBlendModes(instance, systemId, viewConfigType, blendModes.size(), &envBlendModesCount, blendModes.data());
		for (const auto& bm : blendModes) {
			printf("  %d\n", bm);
		}
	}

	XrGraphicsRequirementsOpenGLKHR glReqs{ XR_TYPE_GRAPHICS_REQUIREMENTS_OPENGL_KHR,nullptr };
	r = xrGetOpenGLGraphicsRequirementsKHRd(instance, systemId, &glReqs);
	printf("OpenGL %d.%d required\n", XR_VERSION_MAJOR(glReqs.minApiVersionSupported), XR_VERSION_MINOR(glReqs.minApiVersionSupported));

	XrSession session = XR_NULL_HANDLE;
	XrGraphicsBindingOpenGLWin32KHR glWin32Binding{ XR_TYPE_GRAPHICS_BINDING_OPENGL_WIN32_KHR, nullptr, dc, glrc };
	XrSessionCreateInfo sessionInfo{ XR_TYPE_SESSION_CREATE_INFO, &glWin32Binding, 0, systemId };
	r = xrCreateSession(instance, &sessionInfo, &session);
	if (XR_FAILED(r)) {
		char buffer[XR_MAX_RESULT_STRING_SIZE] = "";
		xrResultToString(XR_NULL_HANDLE, r, buffer);
		printf("xrCreateSession failed %s\n", buffer);
		xrDestroyInstance(instance);
		return -1;
	}

	int64_t swapchainFormat{};

	uint32_t formatsCount = 0;
	r = xrEnumerateSwapchainFormats(session, 0, &formatsCount, nullptr);
	if (formatsCount) {
		printf("Swapchain formats: %d\n", formatsCount);
		std::vector<int64_t>swapchainFormats(formatsCount);
		r = xrEnumerateSwapchainFormats(session, formatsCount, &formatsCount, swapchainFormats.data());
		for (const auto& f : swapchainFormats) {
			printf("  0x%llX\n", f);
		}
		swapchainFormat = swapchainFormats[0];
		for (int i = 0; i < swapchainFormats.size(); i++)
		{
			if (swapchainFormats[i] == GL_SRGB8_ALPHA8)
			{
				swapchainFormat = swapchainFormats[i];
				break;
			}
		}
	}

	XrSwapchain swapchains[2]{};
	std::vector<XrSwapchainImageOpenGLKHR>swapchainImages[2]{};
	XrRect2Di swapchainRect{ {0,0}, {cfgViews[0].recommendedImageRectWidth,cfgViews[0].recommendedImageRectHeight} };
	for (int i = 0; i < cfgViews.size(); i++)
	{
		XrSwapchainCreateInfo swapchainInfo{ XR_TYPE_SWAPCHAIN_CREATE_INFO, nullptr, 0,XR_SWAPCHAIN_USAGE_COLOR_ATTACHMENT_BIT, swapchainFormat,1,
			cfgViews[i].recommendedImageRectWidth,cfgViews[i].recommendedImageRectHeight,1,1,1 };
		r = xrCreateSwapchain(session, &swapchainInfo, swapchains + i);
		if (XR_FAILED(r)) {
			char buffer[XR_MAX_RESULT_STRING_SIZE] = "";
			xrResultToString(XR_NULL_HANDLE, r, buffer);
			printf("xrCreateSwapchain %d failed %s\n", i, buffer);
			xrDestroySession(session);
			xrDestroyInstance(instance);
			return -1;
		}

		uint32_t swapchainImgCount = 0;
		r = xrEnumerateSwapchainImages(swapchains[i], 0, &swapchainImgCount, nullptr);
		if (swapchainImgCount) {
			printf("Swapchain %d images: %d\n", i, swapchainImgCount);
			swapchainImages[i].resize(swapchainImgCount, { XR_TYPE_SWAPCHAIN_IMAGE_OPENGL_KHR,nullptr });
			r = xrEnumerateSwapchainImages(swapchains[i], swapchainImgCount, &swapchainImgCount, (XrSwapchainImageBaseHeader*)swapchainImages[i].data());

			for (const auto& im : swapchainImages[i]) {
				printf("  0x%X\n", im.image);
			}
		}
	}

	uint32_t refSpacesCount = 0;
	r = xrEnumerateReferenceSpaces(session, 0, &refSpacesCount, nullptr);
	if (refSpacesCount) {
		printf("Reference spaces: %d\n", refSpacesCount);
		std::vector<XrReferenceSpaceType> refSpaceTypes(refSpacesCount);
		r = xrEnumerateReferenceSpaces(session, refSpaceTypes.size(), &refSpacesCount, refSpaceTypes.data());
		for (const auto& t : refSpaceTypes) {
			XrExtent2Df bounds{};
			XrResult br = xrGetReferenceSpaceBoundsRect(session, t, &bounds);
			printf("  %d, bounds %fx%f (%d)\n", t, bounds.width, bounds.height, br);
		}
	}

	XrSpace localSpace{};
	XrReferenceSpaceCreateInfo refSpaceCreateInfo{ XR_TYPE_REFERENCE_SPACE_CREATE_INFO,nullptr,XR_REFERENCE_SPACE_TYPE_LOCAL,{{0,0,0,1},{0,0,0}} };
	r = xrCreateReferenceSpace(session, &refSpaceCreateInfo, &localSpace);
	if (XR_FAILED(r)) {
		char buffer[XR_MAX_RESULT_STRING_SIZE] = "";
		xrResultToString(XR_NULL_HANDLE, r, buffer);
		printf("xrCreateReferenceSpace failed %s\n", buffer);
	}

	XrPath handsPaths[2]{};
	r = xrStringToPath(instance, "/user/hand/left", &handsPaths[0]);
	r = xrStringToPath(instance, "/user/hand/right", &handsPaths[1]);
	XrPath gripPaths[2]{};
	r = xrStringToPath(instance, "/user/hand/left/input/grip/pose", &gripPaths[0]);
	r = xrStringToPath(instance, "/user/hand/right/input/grip/pose", &gripPaths[1]);

	XrActionSet actionSet;
	XrActionSetCreateInfo actionSetInfo{ XR_TYPE_ACTION_SET_CREATE_INFO,nullptr,"test1set","Test-1 Action Set",0 };
	r = xrCreateActionSet(instance, &actionSetInfo, &actionSet);
	CheckXrResult(r, "xrCreateActionSet");

	XrAction handAction;
	XrActionCreateInfo actionInfo{ XR_TYPE_ACTION_CREATE_INFO,nullptr,"handpose",XR_ACTION_TYPE_POSE_INPUT,2,handsPaths,"Hand pose" };
	r = xrCreateAction(actionSet, &actionInfo, &handAction);
	CheckXrResult(r, "xrCreateAction");

	XrPath khrControllerPath;
	XrPath viveControllerPath;
	xrStringToPath(instance, "/interaction_profiles/khr/simple_controller", &khrControllerPath);
	xrStringToPath(instance, "/interaction_profiles/htc/vive_controller", &viveControllerPath);
	std::vector<XrActionSuggestedBinding> bindings{ {{handAction, gripPaths[0]},
													{handAction, gripPaths[1]}} };
	XrInteractionProfileSuggestedBinding suggestedBindings{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING, nullptr, viveControllerPath, (uint32_t)bindings.size(), bindings.data() };
	r = xrSuggestInteractionProfileBindings(instance, &suggestedBindings);
	CheckXrResult(r, "xrSuggestInteractionProfileBindings vive");

	suggestedBindings.interactionProfile = khrControllerPath;
	r = xrSuggestInteractionProfileBindings(instance, &suggestedBindings);
	CheckXrResult(r, "xrSuggestInteractionProfileBindings khr");

	XrSpace gripSpaces[2]{};
	XrActionSpaceCreateInfo actionSpaceInfo{ XR_TYPE_ACTION_SPACE_CREATE_INFO,nullptr,handAction,handsPaths[0],{{0,0,0,1},{0,0,0}} };
	r = xrCreateActionSpace(session, &actionSpaceInfo, gripSpaces);
	CheckXrResult(r, "xrCreateActionSpace l");
	actionSpaceInfo.subactionPath = handsPaths[1];
	r = xrCreateActionSpace(session, &actionSpaceInfo, gripSpaces + 1);
	CheckXrResult(r, "xrCreateActionSpace r");

	XrSessionActionSetsAttachInfo attachInfo{ XR_TYPE_SESSION_ACTION_SETS_ATTACH_INFO,nullptr,1,&actionSet };
	r = xrAttachSessionActionSets(session, &attachInfo);
	CheckXrResult(r, "xrAttachSessionActionSets");

	XrSessionState sessionState = XR_SESSION_STATE_UNKNOWN;
	uint64_t renderedFrames = 0;

	std::vector<uint32_t> viewFbos(cfgViews.size());
	std::vector<uint32_t> depthRBs(cfgViews.size());
	glGenFramebuffers(viewFbos.size(), viewFbos.data());
	//glGenRenderbuffers(depthRBs.size(), depthRBs.data());
	for (int i = 0; i < viewFbos.size(); i++) {
		//glBindRenderbuffer(GL_RENDERBUFFER, depthRBs[i]);
		//glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT32, swapchainRect.extent.width, swapchainRect.extent.height);

		glBindFramebuffer(GL_FRAMEBUFFER, viewFbos[i]);
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthRBs[i]);
	}
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);

	uint32_t cubeVAO;
	uint32_t VAOBuffers[2];
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);
	glGenBuffers(2, VAOBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, VAOBuffers[0]);
	glBufferData(GL_ARRAY_BUFFER, 8 * 3 * 4, cubeVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VAOBuffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 24 * 2, cubeInds, GL_STATIC_DRAW);
	//glVertexPointer(3, GL_FLOAT, 12, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 12, 0);
	glBindVertexArray(0);

	std::vector<XrView> views(cfgViews.size(), { XR_TYPE_VIEW,nullptr });
	std::vector<XrCompositionLayerProjectionView> layerProjViews(cfgViews.size());
	XrCompositionLayerProjection layerProjection;
	std::vector<XrCompositionLayerBaseHeader*> frameLayers;
	bool firstTime = true;
	//loop
	XrEventDataBuffer event{ XR_TYPE_EVENT_DATA_BUFFER, nullptr };
	bool shouldClose = false;
	while (!shouldClose) {
		r = xrPollEvent(instance, &event);
		if (r == XR_SUCCESS) {
			char buffer[XR_MAX_STRUCTURE_NAME_SIZE] = "";
			xrStructureTypeToString(instance, event.type, buffer);
			printf("event type %d %s\n", event.type, buffer);
			switch (event.type)
			{
			case XR_TYPE_EVENT_DATA_EVENTS_LOST:
				printf("events lost count %d\n", ((XrEventDataEventsLost*)&event)->lostEventCount);
				break;
			case XR_TYPE_EVENT_DATA_INSTANCE_LOSS_PENDING:
				printf("instance loss pending. Time %lld\n", ((XrEventDataInstanceLossPending*)&event)->lossTime);
				shouldClose = true;
				break;
			case XR_TYPE_EVENT_DATA_SESSION_STATE_CHANGED:
			{
				XrEventDataSessionStateChanged* sessionStateChange = (XrEventDataSessionStateChanged*)&event;
				printf("new session state %d, time %lld\n", sessionStateChange->state, sessionStateChange->time);
				sessionState = sessionStateChange->state;
				if (sessionState == XR_SESSION_STATE_READY) {
					XrSessionBeginInfo sessionBeginInfo{ XR_TYPE_SESSION_BEGIN_INFO,nullptr,viewConfigType };
					XrResult sr = xrBeginSession(session, &sessionBeginInfo);
					if (XR_FAILED(sr)) {
						char buffer[XR_MAX_RESULT_STRING_SIZE] = "";
						xrResultToString(XR_NULL_HANDLE, r, buffer);
						printf("xrBeginSession failed %s\n", buffer);
						shouldClose = true;
					}
				}
				else if (sessionState == XR_SESSION_STATE_STOPPING) {
					xrEndSession(session);
				}
				else if (sessionState == XR_SESSION_STATE_EXITING)
					shouldClose = true;
			}
			break;
			}
		}
		else if (r != XR_EVENT_UNAVAILABLE)
		{
			char buffer[XR_MAX_RESULT_STRING_SIZE] = "";
			xrResultToString(XR_NULL_HANDLE, r, buffer);
			printf("xrPollEvent returned %s\n", buffer);
			if (XR_FAILED(r)) {
				shouldClose = true;
				continue;
			}
		}

		glfwPollEvents();
		if (glfwWindowShouldClose(window))
			xrRequestExitSession(session);

		if (sessionState == XR_SESSION_STATE_READY || sessionState == XR_SESSION_STATE_SYNCHRONIZED
			|| sessionState == XR_SESSION_STATE_VISIBLE || sessionState == XR_SESSION_STATE_FOCUSED) {
			XrFrameWaitInfo frameWaitInfo{ XR_TYPE_FRAME_WAIT_INFO,nullptr };
			XrFrameState frameState{ XR_TYPE_FRAME_STATE,nullptr };
			XrResult wr = xrWaitFrame(session, &frameWaitInfo, &frameState);

			XrFrameBeginInfo frameBeginInfo{ XR_TYPE_FRAME_BEGIN_INFO,nullptr };
			xrBeginFrame(session, &frameBeginInfo);

			XrActiveActionSet syncSet{ actionSet,XR_NULL_PATH };
			XrActionsSyncInfo syncInfo{ XR_TYPE_ACTIONS_SYNC_INFO,nullptr,1,&syncSet };
			xrSyncActions(session, &syncInfo);

			if (frameState.shouldRender) {
				renderedFrames++;

				frameLayers.clear();

				uint32_t swapchainsImgIndex[2]{};
				XrSwapchainImageAcquireInfo acquireInfo{ XR_TYPE_SWAPCHAIN_IMAGE_ACQUIRE_INFO,nullptr };
				for (int i = 0; i < cfgViews.size(); i++) {
					XrResult ar = xrAcquireSwapchainImage(swapchains[i], &acquireInfo, swapchainsImgIndex + i);
					if (XR_FAILED(ar)) {
						char buffer[XR_MAX_RESULT_STRING_SIZE] = "";
						xrResultToString(XR_NULL_HANDLE, ar, buffer);
						printf("xrAcquireSwapchainImage %d failed %s\n", i, buffer);
					}
					/*else {
						printf("Swapchain image acquired: %d\n", swapchainImgIndex);
					}*/

					XrSwapchainImageWaitInfo imageWaitInfo{ XR_TYPE_SWAPCHAIN_IMAGE_WAIT_INFO,nullptr, 100000000 };//100ms
					xrWaitSwapchainImage(swapchains[i], &imageWaitInfo);
				}

				XrViewLocateInfo viewLocateInfo{ XR_TYPE_VIEW_LOCATE_INFO,nullptr,viewConfigType,frameState.predictedDisplayTime, localSpace };
				XrViewState viewState{ XR_TYPE_VIEW_STATE, nullptr };
				uint32_t viewCount = 0;
				xrLocateViews(session, &viewLocateInfo, &viewState, views.size(), &viewCount, views.data());
				if (viewCount)
				{
					//if(viewState.viewStateFlags & XR_VIEW_STATE_ORIENTATION_TRACKED_BIT){
						//views[0].pose
						//views[0].fov
					//}
				}

				XrSpaceLocation spaceLocations[2]{ {XR_TYPE_SPACE_LOCATION} ,{XR_TYPE_SPACE_LOCATION} };
				xrLocateSpace(gripSpaces[0], localSpace, frameState.predictedDisplayTime, spaceLocations);
				xrLocateSpace(gripSpaces[1], localSpace, frameState.predictedDisplayTime, spaceLocations + 1);

				//RENDER!!!
				{
					for (int i = 0; i < views.size(); i++)
					{
						glBindFramebuffer(GL_FRAMEBUFFER, viewFbos[i]);
						glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, swapchainImages[i][swapchainsImgIndex[i]].image, 0);
						glViewport(swapchainRect.offset.x, swapchainRect.offset.y, swapchainRect.extent.width, swapchainRect.extent.height);

						//int i = 0;
						//glBindFramebuffer(GL_FRAMEBUFFER, 0);
						//glViewport(0,0,800,480);

						glClearColor(0.3, 0.4, 0.6, 1.0);
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

						glm::mat4 vpMtx(1);
						vpMtx = glm::translate(vpMtx, glm::make_vec3(&views[i].pose.position.x));
						vpMtx *= glm::mat4(glm::quat(views[i].pose.orientation.w, views[i].pose.orientation.x, views[i].pose.orientation.y, views[i].pose.orientation.z));
						vpMtx = glm::inverse(vpMtx);
						vpMtx = FrustumXR(views[i].fov, 0.01f, 1000.0f) * vpMtx;

						glm::mat4 mvpMtx = vpMtx;

						XREshader->Bind();
						XREshader->SetMat4("u_mvpMtx", mvpMtx);
					
						//glLineWidth(8);

						glBindVertexArray(cubeVAO);
						glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, 0);

						mvpMtx = vpMtx * glm::scale(glm::translate(glm::mat4(1), glm::vec3(0.2, 0.0, -0.3)), glm::vec3(0.1));
						XREshader->SetMat4("u_mvpMtx", mvpMtx);

						glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, 0);

						for (int h = 0; h < 2; h++)
						{
							if (spaceLocations[h].locationFlags & XR_SPACE_LOCATION_POSITION_VALID_BIT)
							{

								mvpMtx = vpMtx * glm::scale(glm::translate(glm::mat4(1), glm::make_vec3(&spaceLocations[h].pose.position.x)) * glm::mat4(glm::make_quat(&spaceLocations[h].pose.orientation.x)), glm::vec3(0.05));
								XREshader->SetMat4("u_mvpMtx", mvpMtx);
								glDrawElements(GL_LINES, 24, GL_UNSIGNED_SHORT, 0);
							}
						}

						//glfwSwapBuffers(window);
					}
					glBindFramebuffer(GL_FRAMEBUFFER, 0);
				}

				XrSwapchainImageReleaseInfo imageReleaseInfo{ XR_TYPE_SWAPCHAIN_IMAGE_RELEASE_INFO, nullptr };
				for (int i = 0; i < cfgViews.size(); i++) {
					r = xrReleaseSwapchainImage(swapchains[i], &imageReleaseInfo);
				}

				for (size_t i = 0; i < views.size(); i++) {
					layerProjViews[i] = { XR_TYPE_COMPOSITION_LAYER_PROJECTION_VIEW, nullptr, views[i].pose, views[i].fov, {swapchains[i],swapchainRect,0} };
				}
				layerProjection = { XR_TYPE_COMPOSITION_LAYER_PROJECTION, nullptr, 0,localSpace,(uint32_t)layerProjViews.size(),layerProjViews.data() };
				frameLayers.push_back((XrCompositionLayerBaseHeader*)&layerProjection);
			}

			XrFrameEndInfo frameEndInfo{ XR_TYPE_FRAME_END_INFO,nullptr,frameState.predictedDisplayTime,XR_ENVIRONMENT_BLEND_MODE_OPAQUE,frameLayers.size(),frameLayers.data() };
			xrEndFrame(session, &frameEndInfo);
		}
	}

	printf("Rendered %lld frames\n", renderedFrames);
	//destroy
	for (size_t i = 0; i < cfgViews.size(); i++) {
		xrDestroySwapchain(swapchains[i]);
	}
	xrDestroySession(session);
	xrDestroyDebugUtilsMessengerEXTd(debugMessenger);
	xrDestroyInstance(instance);

	glfwTerminate();

	printf("Shutting down\n");
	return 0;
}