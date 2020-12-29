#pragma once
#ifndef DRAWFRUSTUM_H
#define DRAWFRUSTUM_H

#include <learnopengl/camera.h>

#include "myFrustum.h"
#include "light.h"
#include "transformation.h"

#include <stack>


static std::stack < glm::mat4> stackModel;
// ---------------------------------------------
// -----------------遍历树形结构----------------
struct treenode {
	// 定义基本图形节点
	glm::mat4 localModel;
	Frustum* object;
	treenode* sibling;
	treenode* child;

	treenode(): localModel(1.0f), object(nullptr), sibling(nullptr), child(nullptr) {}
	treenode(Frustum* object, treenode* sibling, treenode* child, glm::mat4 localModel) {
		this->object = object;
		this->sibling = sibling;
		this->child = child;
		this->localModel = localModel;
	}
};

// -----------------遍历树形结构----------------
void traverse(treenode*node, Transform * transform, Light* light, const Camera& camera) {
	// 从根节点开始遍历
	if (!node) return;
	// 保存之前的矩阵
	stackModel.push(transform->model);
	transform->model = transform->model * node->localModel;
	// 改变之后需要渲染
	node->object->draw(transform->model, transform->view, transform->projection, *light, camera);

	// 支配的节点需要联动
	if (node->child) traverse(node->child, transform, light, camera);
	// 同一级的节点只需要原来的矩阵
	transform->model = stackModel.top();
	stackModel.pop();
	if (node->sibling) traverse(node->sibling, transform, light, camera);
}



#endif // !DRAWFRUSTUM_H

