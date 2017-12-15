#pragma once

#include <JKCommon\JKGuid.h>



#define JK_BLL_INIT(T) \
protected:\
T##BLL(JKRef_Ptr<JKBLLContext> _refContext) : JKBaseBLL(_refContext) { ref##T##Model = SingleDB->createBean<T##Model>(); \
ref##T##Model->id = JKGuid::GuidToString(JKGuid::CreateGuid());};\
T##BLL(bean_ptr<T##Model> _refModel, JKRef_Ptr<JKBLLContext> _refContext) : JKBaseBLL(_refContext) { ref##T##Model = _refModel; }\
bean_ptr<T##Model> getModel() { return ref##T##Model; };\
bean_ptr<T##Model> ref##T##Model;\
public:\
const JKString & getId() { return ref##T##Model->id; }\


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


