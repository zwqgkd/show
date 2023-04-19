#include <jni.h>
#include "object.h"
#include "base64.h"
#include<iostream>
#include<vector>
#include<string>

#define EXPORT extern "C" __declspec(dllexport)


jstring stoJstring(JNIEnv* env, const char* pat)
{
	jclass strClass = env->FindClass("Ljava/lang/String;");
	jmethodID ctorID = env->GetMethodID(strClass, "<init>", "([BLjava/lang/String;)V");
	jbyteArray bytes = env->NewByteArray(strlen(pat));
	env->SetByteArrayRegion(bytes, 0, strlen(pat), (jbyte*)pat);
	jstring encoding = env->NewStringUTF("utf-8");
	return (jstring)env->NewObject(strClass, ctorID, bytes, encoding);
}


jintArray vectorInt2jobjectArray(JNIEnv* env, std::vector<int> v)
{

	jintArray jintArr = env->NewIntArray(v.size());
	env->SetIntArrayRegion(jintArr, 0, v.size(), (jint*)v.data());
	return jintArr;
}


jdoubleArray vectorDouble2jobjectArray(JNIEnv* env, std::vector<double> v)
{


	jdoubleArray jdoubleArr = env->NewDoubleArray(v.size());
	env->SetDoubleArrayRegion(jdoubleArr, 0, v.size(), v.data());
	return jdoubleArr;
}


jobjectArray vectorStringjobjectArray(JNIEnv* env, std::vector<std::string> v)
{
	jobjectArray jobjArr = env->NewObjectArray(v.size(), env->FindClass("java/lang/String"), nullptr);
	for (int i = 0; i < v.size(); i++) {
		jstring str = env->NewStringUTF(v[i].c_str());
		env->SetObjectArrayElement(jobjArr, i, str);
		env->DeleteLocalRef(str);
	}
	return jobjArr;
}


std::string mat_to_base64(const cv::Mat& img) {
	std::vector<uchar> buf;
	cv::imencode(".jpg", img, buf);
	auto* enc_msg = reinterpret_cast<unsigned char*>(buf.data());
	return base64_encode(enc_msg, buf.size());
}

EXPORT void show(JNIEnv* ENV, jobject LISTENER, jmethodID METHOD, ParamPtrArray& params) {

	int typeMode = get_data<int>(params[1]);

	cv::Mat mat_dst;
	std::vector<std::string> s_dst;
	std::vector<int> i_dst;
	std::vector<double> d_dst;

	jintArray intArray;
	jdoubleArray doubleArray;
	jobjectArray stringArray;



	switch (typeMode) {
	case 0:
		mat_dst = get_data<cv::Mat>(params[0]);
		ENV->CallVoidMethod(LISTENER, METHOD, 0, stoJstring(ENV, mat_to_base64(mat_dst).c_str()), new int(), new double(), new std::string());
		break;
	case 1:
	{
		i_dst = get_data<std::vector<int>>(params[0]);
		intArray = vectorInt2jobjectArray(ENV, i_dst);
		if (intArray == NULL) std::cout << "jintA null" << std::endl;
		ENV->CallVoidMethod(LISTENER, METHOD, 1, "", intArray, new double(), new std::string());
		break;
	}
	case 2:
	{
		d_dst = get_data<std::vector<double>>(params[0]);
		doubleArray = vectorDouble2jobjectArray(ENV, d_dst);
		if (doubleArray == NULL) std::cout << "jdoubleA null" << std::endl;
		ENV->CallVoidMethod(LISTENER, METHOD, 2, "", new int(), doubleArray, new std::string());
		break;
	}
	case 3:
	{
		s_dst = get_data<std::vector<std::string>>(params[0]);
		stringArray = vectorStringjobjectArray(ENV, s_dst);
		if (stringArray == NULL) std::cout << "jstringA null" << std::endl;
		ENV->CallVoidMethod(LISTENER, METHOD, 3, "", new int(), new double(), stringArray);
		break;
	}

	}
}