#pragma once


#define JK_BLL_INIT(T) \
template<typename T>\
friend class JKBLLContainer;\
protected:\
	T##BLL(JKInt _parentID = -1) : JKBaseBLL<T##Model>(_parentID) {};\
	T##BLL(bean_ptr<T##Model> _refModel) : JKBaseBLL<T##Model>() \
	{ \
		ptrModel = _refModel; \
	}\



#define NewBLLObject(obj) JKRef_Ptr<JK##obj##BLL> _ref##obj = new JK##obj##BLL();\
	_ref##obj->setContext(refContext);

//{
//protected:
//	JKProjectBLL(JKRef_Ptr<JKBLLContext> _refContext) 
//	 : JKBaseBLL(_refContext) 
//	{
//		refJKProjectModel = SingleDB->createBean<JKProjectModel>(); 
//		refJKProjectModel->id = JKGuid::GuidToString(JKGuid::CreateGuid());
//	}
//	JKProjectBLL(bean_ptr<JKProjectBLLModel> _refModel) { refJKProjectBLLModel = _refModel; }
//	bean_ptr<JKProjectBLLModel> getModel() { return refJKProjectBLLModel; };
//	bean_ptr<JKProjectBLLModel> refJKProjectBLLModel;
//public:
//	const JKString & getId() { return refJKProjectBLLModel->id; }
//}


