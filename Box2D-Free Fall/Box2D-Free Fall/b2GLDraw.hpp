//
//  b2GLDraw.hpp
//  Box2D-Free Fall
//
//  Created by phani srikar on 10/04/20.
//  Copyright © 2020 phani srikar. All rights reserved.
//

#ifndef RENDER_H
#define RENDER_H

#include <GLUT/GLUT.h>
#include <box2d/box2d.h>

struct b2AABB;

class b2GLDraw : public b2Draw {
public:
    void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    
    void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color);
    
    void DrawCircle(const b2Vec2& center, float radius, const b2Color& color);
    
    void DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color);
    
    void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color);
    
    void DrawTransform(const b2Transform& xf);
    
    void DrawPoint(const b2Vec2& p, float size, const b2Color& color);
    
    void DrawString(int x, int y, const char* string, ...);
    
    void DrawAABB(b2AABB* aabb, const b2Color& color);
};

#endif
