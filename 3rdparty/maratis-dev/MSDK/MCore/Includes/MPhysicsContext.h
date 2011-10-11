/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MCore
// MPhysicsContext.h
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


#ifndef _M_PHYSICS_CONTEXT_H
#define _M_PHYSICS_CONTEXT_H

typedef void * MPhysicsObjectId;
typedef void * MPhysicsShapeId;
typedef void * MPhysicsConstraintId;

class M_CORE_EXPORT MPhysicsContext
{
public:
	typedef MPhysicsObjectId ObjectId;
	typedef MPhysicsShapeId ShapeId;
	typedef MPhysicsConstraintId ConstraintId;

	// destructor
	virtual ~MPhysicsContext(void){}

	// init
	virtual void init(const MVector3 & worldMin, const MVector3 & worldMax) = 0;

	// destroy
	virtual void clear(void) = 0;

	// update simulation
	virtual void updateSimulation(void) = 0;

	// world
	virtual void setWorldGravity(const MVector3 & gravity) = 0;

	// create object
	virtual void createGhost(ObjectId * objectId, ShapeId shapeId, const MVector3 & position, const MQuaternion & rotation) = 0;
	virtual void createRigidBody(ObjectId * objectId, ShapeId shapeId, const MVector3 & position, const MQuaternion & rotation, float mass) = 0;

	// delete object
	virtual void deleteObject(ObjectId * objectId) = 0;

	// object properties
	virtual void enableObjectKinematic(ObjectId objectId) = 0;
	virtual void disableObjectKinematic(ObjectId objectId) = 0;

	virtual void setObjectShape(ObjectId objectId, ShapeId shapeId) = 0;
	virtual void setObjectMass(ObjectId objectId, float mass) = 0;
	virtual void setObjectRestitution(ObjectId objectId, float restitution) = 0;
	virtual void setObjectFriction(ObjectId objectId, float friction) = 0;
	virtual void setObjectLinearFactor(ObjectId objectId, const MVector3 & linearFactor) = 0;
	virtual void setObjectAngularFactor(ObjectId objectId, float angularFactor) = 0;
	virtual void setObjectDamping(ObjectId objectId, float linearDamping, float angularDamping) = 0;
	virtual void setObjectTransform(ObjectId objectId, const MVector3 & position, const MQuaternion & rotation) = 0;
	virtual void getObjectTransform(ObjectId objectId, MVector3 * position, MQuaternion * rotation) = 0;
	virtual void setUserPointer(ObjectId objectId, void *userPointer) = 0;
	virtual void * getUserPointer(ObjectId objectId) = 0;

	// affectors
	virtual void addCentralForce(ObjectId objectId, const MVector3 & force) = 0;
	virtual void getCentralForce(ObjectId objectId, MVector3 * force) = 0;
	virtual void addTorque(ObjectId objectId, const MVector3 & torque) = 0;
	virtual void getTorque(ObjectId objectId, MVector3 * torque) = 0;
	virtual void clearForces(ObjectId objectId) = 0;

	// objects collision
	virtual int isObjectInCollision(ObjectId objectId) = 0;
	virtual bool isObjectsCollision(ObjectId object1Id, ObjectId object2Id) = 0;
	virtual void getCollidingObjectPairs(vector<pair<ObjectId, ObjectId> >* pairs) = 0;

	// create shape
	virtual void createMultiShape(ShapeId * shapeId) = 0;

	virtual void createBoxShape(ShapeId * shapeId, const MVector3 & scale) = 0;
	virtual void createSphereShape(ShapeId * shapeId, float radius) = 0;
	virtual void createConeShape(ShapeId * shapeId, float radius, float height) = 0;
	virtual void createCapsuleShape(ShapeId * shapeId, float radius, float height) = 0;
	virtual void createCylinderShape(ShapeId * shapeId, float radius, float height) = 0;

	virtual void createConvexHullShape(ShapeId * shapeId, const MVector3 * vertices, unsigned int verticesNumber, const MVector3 scale = MVector3(1)) = 0;
	virtual void createTriangleMeshShape(ShapeId * shapeId, const MVector3 * vertices, unsigned int verticesNumber, const void * indices, unsigned int indicesNumber, M_TYPES indicesType, const MVector3 scale = MVector3(1)) = 0;

	// delete shape
	virtual void deleteShape(ShapeId * shapeId) = 0;

	// add child shape to multishape
	virtual void addChildShape(ShapeId multiShapeId, ShapeId shapeId, const MVector3 & position, const MQuaternion & rotation) = 0;

	// create constraint
	virtual void createConstraint(ConstraintId * constraintId, ObjectId parentObjectId, ObjectId objectId, const MVector3 & pivot, bool disableParentCollision) = 0;
	virtual void setLinearLimit(ConstraintId constraintId, const MVector3 & lower, const MVector3 & upper) = 0;
	virtual void setAngularLimit(ConstraintId constraintId, const MVector3 & lower, const MVector3 & upper) = 0;
	
	// delete constraint
	virtual void deleteConstraint(ConstraintId * constraintId) = 0;
};

#endif