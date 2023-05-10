#include <jni.h>
#include "object.h"
#include "base64.h"
#include<iostream>
#include<vector>
#include<unordered_map>
#include<string>
#include<vector>
#include "fmt/core.h"

#define EXPORT extern "C" __declspec(dllexport)

std::string mat_to_base64(const cv::Mat& img) {
	std::vector<uchar> buf;
	cv::imencode(".jpg", img, buf);
	auto* enc_msg = reinterpret_cast<unsigned char*>(buf.data());
	return base64_encode(enc_msg, buf.size());
}

std::string point_to_string(const cv::Point& point ) {
	return fmt::format(R"({{"type": "Point", "content": "{},{}"}})", point.x, point.y);
}

std::string rect_to_string(const cv::Rect& rect) {
	return fmt::format(R"({{"type": "Rect", "content": "{},{},{},{}"}})", rect.x,rect.y,rect.width,rect.height);
}



jstring stoJstring(JNIEnv* env, const char* pat)
{
	jclass strClass = env->FindClass("Ljava/lang/String;");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
	jstring encoding = env->NewStringUTF("utf-8");
	return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}


std::unordered_map<std::string, std::string> explanation_to_event;



EXPORT void show(JNIEnv* ENV, jobject LISTENER, jmethodID METHOD,std::string type,std::string explanation, ParamPtrArray& params) {

	std::string eventName;
	//eventName = explanation_to_event.at(explanation);
;
	std::string msg = "";

	std::cout << "from show.dll:" << std::endl << "the outType is: " << type << std::endl << "the explanation is: " << explanation << std::endl;

	if (type == "Mat") {
		eventName = "revBase64";
		msg += "data:image/jpeg;base64,";
		cv::Mat dst= get_data<cv::Mat>(params[0]);
		msg += mat_to_base64(dst).c_str();
	}
	else if (type == "string") {
		eventName = "revStr";
		msg = get_data<std::string>(params[0]);
	}
	else if (type == "Point") {
		eventName = "revPoint";
		std::vector<cv::Point> vec_point = get_data<std::vector<cv::Point>>(params[0]);
		for(cv::Point p:vec_point)
			msg+=point_to_string(p);
	}
	else if (type == "Rect") {
		eventName = "revRect";
		cv::Rect point = get_data<cv::Rect>(params[0]);
		msg = rect_to_string(point);
	}
	ENV->CallVoidMethod(LISTENER, METHOD, stoJstring(ENV, eventName.c_str()), stoJstring(ENV, msg.c_str()));
}