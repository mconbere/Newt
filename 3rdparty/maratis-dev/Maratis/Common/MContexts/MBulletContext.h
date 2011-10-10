/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MCore
// MBulletContext.h
//
// Bullet Physics Context 
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2003-2011 Anael Seghezzi <www.maratis3d.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================


#ifndef _M_BULLET_CONTEXT_H
#define _M_BULLET_CONTEXT_H

#include <btBulletDynamicsCommon.h>
#include <MCore.h>
#include <set>

class MBulletContext : public MPhysicsContext
{
private:

	btDefaultCollisionConfiguration * m_collisionConfiguration;
	btCollisionDispatcher * m_dispatcher;
	//btAxisSweep3 * m_overlappingPairCache;
	btDbvtBroadphase * m_overlappingPairCache;
	btSequentialImpulseConstraintSolver * m_solver;
	btDiscreteDynamicsWorld * m_dynamicsWorld;

	// bullet does not allow access to the set of all shapes, so keep track ourselves
	set<btCollisionShape*> m_collisionShapes;

public:

	// constructor
	MBulletContext(void);

	// destructor
	~MBulletContext(void);

	// init
	void init(const MVector3 & worldMin, const MVector3 & worldMax);

	// clear
	void clear(void);

	// update simulation
	void updateSimulation(void);

	// world gravity
	void setWorldGravity(const MVector3 & gravity);

	// create object
	void createGhost(ObjectId * objectId, ShapeId shapeId, const MVector3 & position, const MQuaternion & rotation);
	void createRigidBody(ObjectId * objectId, ShapeId shapeId, const MVector3 & position, const MQuaternion & rotation, float mass);

	// delete object
	void deleteObject(ObjectId * objectId);

	// object properties
	void enableObjectKinematic(ObjectId objectId);
	void disableObjectKinematic(ObjectId objectId);

	void setObjectShape(ObjectId objectId, ShapeId shapeId);
	void setObjectMass(ObjectId objectId, float mass);
	void setObjectRestitution(ObjectId objectId, float restitution);
	void setObjectFriction(ObjectId objectId, float friction);
	void setObjectLinearFactor(ObjectId objectId, const MVector3 & linearFactor);
	void setObjectAngularFactor(ObjectId objectId, float angularFactor);
	void setObjectDamping(ObjectId objectId, float linearDamping, float angularDamping);
	void setObjectTransform(ObjectId objectId, const MVector3 & position, const MQuaternion & rotation);
	void getObjectTransform(ObjectId objectId, MVector3 * position, MQuaternion * rotation);
	void setUserPointer(ObjectId objectId, void *userPointer);
	void * getUserPointer(ObjectId objectId);

	// affectors
	void addCentralForce(ObjectId objectId, const MVector3 & force);
	void getCentralForce(ObjectId objectId, MVector3 * force);
	void addTorque(ObjectId objectId, const MVector3 & torque);
	void getTorque(ObjectId objectId, MVector3 * torque);
	void clearForces(ObjectId objectId);

	// objects collision
	int isObjectInCollision(ObjectId objectId);
	bool isObjectsCollision(ObjectId objectId, ObjectId object2Id);
	void forEachObjectColliding(objectCollidingCallback callback);

	// create shape
	void createMultiShape(ShapeId * shapeId);

	void createBoxShape(ShapeId * shapeId, const MVector3 & scale);
	void createSphereShape(ShapeId * shapeId, float radius);
	void createConeShape(ShapeId * shapeId, float radius, float height);
	void createCapsuleShape(ShapeId * shapeId, float radius, float height);
	void createCylinderShape(ShapeId * shapeId, float radius, float height);

	void createConvexHullShape(ShapeId * shapeId, const MVector3 * vertices, unsigned int verticesNumber, const MVector3 scale = MVector3(1));
	void createTriangleMeshShape(ShapeId * shapeId, const MVector3 * vertices, unsigned int verticesNumber, const void * indices, unsigned int indicesNumber, M_TYPES indicesType, const MVector3 scale = MVector3(1));

	// delete shape
	void deleteShape(ShapeId * shapeId);

	// add child shape to multishape
	void addChildShape(ShapeId multiShapeId, ShapeId shapeId, const MVector3 & position, const MQuaternion & rotation);

	// create constraint
	void createConstraint(ConstraintId * constraintId, ObjectId parentObjectId, ObjectId objectId, const MVector3 & pivot, bool disableParentCollision);
	void setLinearLimit(ConstraintId constraintId, const MVector3 & lower, const MVector3 & upper);
	void setAngularLimit(ConstraintId constraintId, const MVector3 & lower, const MVector3 & upper);
	
	// delete constraint
	void deleteConstraint(ConstraintId * constraintId);
};

#endif