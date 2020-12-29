#pragma once
#ifndef MYTREE_H
#define MYTREE_H

#include "myFrustum.h"
#include "drawFrustum.h"

typedef glm::vec3 Position;

class MyTree {
public:
	Frustum stem;
	Frustum leafTop;
	Frustum leafLower;
	
	Position pStem;
	Position pLeafTop;
	Position pLeafLower;

	std::vector<treenode> nodes;

	MyTree(float base = 1.0f) :
		stem(0.25f * base, 0.25f * base, 0.5f * base, glm::vec3(0.64f, 0.42f, 0.44f)), // 初始化形状
		leafTop(0.01f * base, 0.5f * base, 0.5f * base, glm::vec3(0, 0.9f, 0)),
		leafLower(0.25f * base, 0.5f * base, 0.25f * base, glm::vec3(0, 0.9f, 0)),
		nodes(3)
	{
		// 首先设置**初始相对**位置，注意都是从原点开始，形状已经设置好
		pStem = glm::vec3(0.0f, 0.5 * stem.getHeight(), 0.0f);
		pLeafLower = glm::vec3(0.0f, 0.5 * stem.getHeight() +  0.5 * leafLower.getHeight(), 0.0f);
		pLeafTop = glm::vec3(0.0f, 0.5 * leafLower.getHeight() +  0.5 * leafTop.getHeight(), 0.0f);

		// 建立树型结构

		// 设置基本变换矩阵，初始化变量
		// stem
		nodes[0].localModel = glm::translate(nodes[0].localModel, pStem);
		nodes[0].object = &stem;
		nodes[0].child = &nodes[1];
		// lowerleaf
		nodes[1].localModel = glm::translate(nodes[1].localModel, pLeafLower);
		nodes[1].object = &leafLower;
		nodes[1].child = &nodes[2];
		// uppperleaf
		nodes[2].localModel = glm::translate(nodes[2].localModel, pLeafTop);
		nodes[2].object = &leafTop;

	}

	void draw(Transform* transform, Light* light, const Camera& camera) {
		traverse(&nodes[0], transform, light, camera);
	}


	void translate(glm::vec3 direction) {
		nodes[0].localModel = glm::translate(nodes[0].localModel, direction);
	}
	
};


#endif // !MYTREE_H

