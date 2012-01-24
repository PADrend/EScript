// ExtObject.cpp
// This file is part of the EScript programming language.
// See copyright notice in EScript.h
// ------------------------------------------------------
#include "ExtObject.h"

#include "../EScript.h"

using namespace EScript;

//! (static)
Type * ExtObject::getTypeObject(){
	// [ExtObject] ---|> [Object]
	static Type * typeObject=new Type(Object::getTypeObject());
	return typeObject;
}

//! (static) initMembers
void ExtObject::init(EScript::Namespace & globals) {
	Type * typeObject=getTypeObject();
	declareConstant(&globals,getClassName(),typeObject);

	//!	[ESF] ExtObject new ExtObject( [Map objAttributes] )
	ES_FUNCTION_DECLARE(typeObject,"_constructor",0,1,{
		ERef<ExtObject> result(new ExtObject(assertType<Type>(runtime,caller)));
		if(parameter.count()>0){
			Map * m=assertType<Map>(runtime,parameter[0]);
			for(Map::const_iterator it=m->begin();it!=m->end();++it)
				result->setAttribute( it->first.c_str(), it->second.value );
		}
		return result.detachAndDecrease();
	})

}

// -----------------------------------------------------------------------------------------------

//! (static) factory
ExtObject * ExtObject::create(){
	return new ExtObject();
}


//! (ctor)
ExtObject::ExtObject():
		Object(ExtObject::getTypeObject()),objAttributes(NULL) {
	//ctor
}

//! (ctor)
ExtObject::ExtObject(Type * parentType):
		Object(parentType),objAttributes(NULL) {
	if (typeRef.isNull())
		return;

	typeRef->initInstanceObjAttributes(this);
	//ctor
}

//! (dtor)
ExtObject::~ExtObject() {
	delete objAttributes;
	objAttributes=NULL;
	//dtor
}

//! ---|> [Object]
Object * ExtObject::clone() const{
	ExtObject * c=new ExtObject(getType());
	c->cloneAttributesFrom(this);
	return c;
}

// -----------------------------------------------------------------------------------------------
// attributes

//! ---o
Attribute * ExtObject::_accessAttribute(const identifierId id,bool localOnly){
	if(objAttributes!=NULL){
		attributeMap_t::iterator f=objAttributes->find(id);
		if( f!=objAttributes->end() ){
			return &f->second;
		}
	}
	return (localOnly||getType()==NULL) ? NULL : getType()->findTypeAttribute(id);
}
//
//Object * ExtObject::getObjAttribute(const identifierId id)const{
//	if(objAttributes!=NULL){
//		attributeMap_t::const_iterator f=objAttributes->find(id);
//		if( f!=objAttributes->end() ){
//			return f->second.getValue();
//		}
//	}
//	return NULL;
//}
//
////! ---|> [Object]
//Object * ExtObject::getAttribute(const identifierId id){
//	if(objAttributes!=NULL){
//		attributeMap_t::const_iterator f=objAttributes->find(id);
//		if( f!=objAttributes->end() )
//			return f->second.getValue();
//	}
//	return Object::getAttribute(id);
//}

//! ---|> [Object]
bool ExtObject::setAttribute(const identifierId id,const Attribute & attr){
	if(objAttributes == NULL)
		objAttributes = new attributeMap_t();
	(*objAttributes)[id] = attr;
	return true;
}

////! ---|> [Object]
//bool ExtObject::assignAttribute(Runtime & rt,const identifierId id,ObjPtr val){
//	return assignObjAttribute(rt,id,val) ? true :  Object::assignAttribute(rt,id,val);
//}
//
//
//bool ExtObject::assignObjAttribute(Runtime & rt,const identifierId id,ObjPtr val){
//	if(objAttributes!=NULL){
//		attributeMap_t::iterator it=objAttributes->find(id);
//		if(it!=objAttributes->end()){
//			Attribute & attr = it->second;
//			std::cout << ":"<<identifierIdToString(id)<<" = "<< val.toString()<<"("+(int)attr.getFlags()<<")\n";
//			if(attr.isConst()){
//				throw new Exception("trying to assign to a const attribute.");
//			}
//			attr.setValue(val.get());
//			return true;
//		}
//	}
//	return false;
//}


void ExtObject::cloneAttributesFrom(const ExtObject * obj) {
	if(obj->objAttributes==NULL)
		return;

	for(attributeMap_t::iterator it=obj->objAttributes->begin() ; it!=obj->objAttributes->end() ; ++it){
		setAttribute(it->first, Attribute(it->second.getValue()->getRefOrCopy(),it->second.getFlags() ));
	}
}

//! ---|> Object
void ExtObject::getLocalAttributes(std::map<identifierId,Object *> & attrs){
	if(objAttributes!=NULL){
		for(attributeMap_t::iterator it=objAttributes->begin() ; it!=objAttributes->end() ; ++it){
			attrs[it->first] = it->second.getValue();
		}
	}
}
