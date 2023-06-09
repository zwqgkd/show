#pragma once
#include"opencv2/opencv.hpp"
struct OperatorInterfaceBase {
public:
	virtual ~OperatorInterfaceBase() {}
	virtual std::string to_string() = 0;
};

template<typename T>
struct OperatorInterface : public OperatorInterfaceBase {
public:
	OperatorInterface(const std::string &name, const std::string &type, T* ptr);
	~OperatorInterface();

	T		 data();
	T&		 data_ref();
	const T& data_const_ref();
	const std::string type() { return type; }
private:
	const std::string	name_;
	const std::string	type_;
	T* ptr;
};

using MatObject = OperatorInterface<cv::Mat>;
using SizeObject = OperatorInterface<cv::Size>;
using IntObject = OperatorInterface<int>;
using DoubleObject = OperatorInterface<double>;
using StringObject = OperatorInterface<std::string>;

using ParamPtr = std::shared_ptr<OperatorInterfaceBase>;
using ParamPtrArray = std::vector<ParamPtr>;

template<typename T>
OperatorInterface<T>::OperatorInterface(const std::string &name, const std::string &type, T* ptr) :
	name_(name), type_(type), ptr(ptr) {}

template<typename T>
OperatorInterface<T>::~OperatorInterface() {
	delete ptr;
}

template<typename T>
T OperatorInterface<T>::data() {
	return *ptr;
}

template<typename T>
const T& OperatorInterface<T>::data_const_ref() {
	return *ptr;
}

template<typename T>
T& OperatorInterface<T>::data_ref() {
	return *ptr;
}


template<typename T>
T get_data(ParamPtr param_ptr) {
	return dynamic_cast<OperatorInterface<T>*>(param_ptr.get())->data();
}

template<typename T>
T& get_data_ref(ParamPtr param_ptr) {
	return dynamic_cast<OperatorInterface<T>*>(param_ptr.get())->data_ref();
}

template<typename T>
const T& get_data_const_ref(ParamPtr param_ptr) {
	return dynamic_cast<OperatorInterface<T>*>(param_ptr.get())->data_const_ref();
}

template<typename T>
const T& get_type(ParamPtr param_ptr) {
	return dynamic_cast<OperatorInterface<T>*>(param_ptr.get())->type();
}

template<typename T>
ParamPtr make_param(std::string name, std::string type, T* value_ptr) {
	return ParamPtr(new OperatorInterface<T>(name, type, value_ptr));
}

//template <> inline std::string OperatorInterface<std::string>::to_string() {
// return fmt::format(R"({{"type": "string", "content": "{}"}})", this->data_const_ref());
//}
//
//template <> inline std::string OperatorInterface<int>::to_string() {
// return fmt::format(R"({{"type": "int", "content": "{}"}})", this->data());
//}
//
//template <> inline std::string OperatorInterface<double>::to_string() {
// return fmt::format(R"({{"type": "double", "content": "{}"}})", this->data());
//}
//
//template <> inline std::string OperatorInterface<cv::Size>::to_string() {
// return fmt::format(R"({{"type": "Size", "content": "{},{}"}})", this->data().width, this->data().height);
//}

