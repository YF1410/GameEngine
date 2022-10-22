#include "ElementObject.h"

ElementObject::ElementObject() {

}

ElementObject::~ElementObject() {

}

std::unique_ptr<ElementObject> ElementObject::Create(FbxModel* fbxmodel, XMFLOAT3 pos) {
	std::unique_ptr<ElementObject> element = std::make_unique<ElementObject>();

	element->Initialize();
	element->SetModel(fbxmodel);
	XMFLOAT3 elementPos = { pos.x,0,pos.z };
	element->SetPosition(elementPos);
	element->SetRotation({ 0,90.0f,0, });
	element->SetColor({ 0,0,1,1 });
	element->collision.center = XMLoadFloat3(&elementPos);
	element->collision.radius = 1.0f;
	element->LoopAnimation();

	return element;
}