#pragma once


#define JK_BLL_INIT(T) \
protected:\
T##BLL() { ref##T##Model = SingleDB.createBean<T##Model>(); };\
T##BLL(bean_ptr<T##Model> _refModel) { ref##T##Model = _refModel; }\
bean_ptr<T##Model> getModel() { return ref##T##Model; };\
bean_ptr<T##Model> ref##T##Model;

