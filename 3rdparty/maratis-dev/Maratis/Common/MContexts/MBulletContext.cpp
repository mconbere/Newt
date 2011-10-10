/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MCore
// MBulletContext.cpp
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


#include <stdio.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include "MBulletContext.h"


// constructor
MBulletContext::MBulletContext(void):
	m_collisionConfiguration(NULL),
	m_dispatcher(NULL),
	m_overlappingPairCache(NULL),
	m_solver(NULL),
	m_dynamicsWorld(NULL)
{
}

// destructor
MBulletContext::~MBulletContext(void)
{
	clear();
}

// init
void MBulletContext::init(const MVector3 & worldMin, const MVector3 & worldMax)
{
	clear();

	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);

	//int	maxProxies = 1024;
	btVector3 worldAabbMin(worldMin.x, worldMin.y, worldMin.z);
	btVector3 worldAabbMax(worldMax.x, worldMax.y, worldMax.z);
	//m_overlappingPairCache = new btAxisSweep3(worldAabbMin, worldAabbMax, maxProxies);
	m_overlappingPairCache = new btDbvtBroadphase();
	
	m_solver = new btSequentialImpulseConstraintSolver();
	m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_solver, m_collisionConfiguration);
	m_dynamicsWorld->setGravity(btVector3(0,0,-10));
}

// destroy
void MBulletContext::clear(void)
{
	// delete constraints
	if(m_dynamicsWorld) {
		size_t numConstraints = m_dynamicsWorld->getNumConstraints();
		vector<ConstraintId> constraints(numConstraints);
		for(size_t i=0; i<numConstraints; i++){
			constraints.push_back(m_dynamicsWorld->getConstraint((int)i));
		}
		for(vector<ConstraintId>::iterator it = constraints.begin(); it != constraints.end(); ++it) {
			deleteConstraint(&(*it));
		}
	}

	if(m_dynamicsWorld) {
		// make an explicit copy
		btCollisionObjectArray collisionObjects = m_dynamicsWorld->getCollisionObjectArray();
		for(int i=0; i<collisionObjects.size(); i++){
		btCollisionObject *object = collisionObjects[i];
			deleteObject((ObjectId *)&object);
		}
	}

	// delete collision shapes
	for(set<btCollisionShape*>::iterator it=m_collisionShapes.begin(); it!=m_collisionShapes.end(); ++it) {
		ShapeId id = *it;
		deleteShape(&id);
	}

	m_collisionShapes.clear();

	SAFE_DELETE(m_dynamicsWorld);
	SAFE_DELETE(m_solver);
	SAFE_DELETE(m_overlappingPairCache);
	SAFE_DELETE(m_dispatcher);
	SAFE_DELETE(m_collisionConfiguration);
}

// update simulation
void MBulletContext::updateSimulation(void)
{
	//m_dynamicsWorld->stepSimulation(1.0f/6.0f, 1, 1.0f/6.0f); // simple quality
	m_dynamicsWorld->stepSimulation(1.0f/6.0f, 2, 1.0f/12.0f); // double quality
	//m_dynamicsWorld->stepSimulation(1.0f/6.0f, 4, 1.0f/24.0f); // etc
}

// world
void MBulletContext::setWorldGravity(const MVector3 & gravity){
	m_dynamicsWorld->setGravity(btVector3(gravity.x, gravity.y, gravity.z));
}

// create object
void MBulletContext::createGhost(ObjectId * objectId, ShapeId shapeId, const MVector3 & position, const MQuaternion & rotation)
{
	createRigidBody(objectId, shapeId, position, rotation, 0.0000001f);
 
	btCollisionObject *object = (btCollisionObject *)*objectId;
	object->setCollisionFlags(object->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE | btCollisionObject::CF_KINEMATIC_OBJECT);
}

void MBulletContext::createRigidBody(ObjectId * objectId, ShapeId shapeId, const MVector3 & position, const MQuaternion & rotation, float mass)
{
	btCollisionShape * shape = (btCollisionShape *)shapeId;

	btVector3 localInertia(0,0,0);
	if(mass > 0)
		shape->calculateLocalInertia(mass, localInertia);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(position.x, position.y, position.z));
	startTransform.setRotation(
		btQuaternion(rotation.values[0], rotation.values[1], rotation.values[2], rotation.values[3])
		);

	btDefaultMotionState * motionState = new btDefaultMotionState(startTransform);
	btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, shape, localInertia);

	btRigidBody * rigidBody = new btRigidBody(info);
	rigidBody->setSleepingThresholds(0.2f, 0.2f); // default : 0.8f, 1.0f

	m_dynamicsWorld->addRigidBody(rigidBody);
	*objectId = rigidBody;
}

// delete object
void MBulletContext::deleteObject(ObjectId * objectId)
{
	btCollisionObject * object = *((btCollisionObject **)objectId);
	if(object)
	{
		if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
		{
			btRigidBody * body = btRigidBody::upcast(object);
			if(body && body->getMotionState()){
				btMotionState * motionState = body->getMotionState();
				SAFE_DELETE(motionState);
			}
		}

		m_dynamicsWorld->removeCollisionObject(object);
		SAFE_DELETE(object);
		*objectId = NULL;
	}
}

// object properties
void MBulletContext::enableObjectKinematic(ObjectId objectId)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	object->setCollisionFlags(object->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
}

void MBulletContext::disableObjectKinematic(ObjectId objectId){

}

void MBulletContext::setObjectShape(ObjectId objectId, ShapeId shapeId)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	btCollisionShape * shape = (btCollisionShape *)shapeId;
	object->setCollisionShape(shape);
}

void MBulletContext::setObjectMass(ObjectId objectId, float mass)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);
		body->setMassProps(mass, btVector3(0, 0, 0));
	}
}

void MBulletContext::setObjectRestitution(ObjectId objectId, float restitution)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	object->setRestitution(btScalar(restitution));
}

void MBulletContext::setObjectFriction(ObjectId objectId, float friction)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object){
		object->setFriction(friction);
	}
}

void MBulletContext::setObjectLinearFactor(ObjectId objectId, const MVector3 & linearFactor)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);
		body->setLinearFactor(btVector3(linearFactor.x, linearFactor.y, linearFactor.z));
	}
}

void MBulletContext::setObjectAngularFactor(ObjectId objectId, float angularFactor)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);
		body->setAngularFactor(btScalar(angularFactor));
	}
}

void MBulletContext::setObjectDamping(ObjectId objectId, float linearDamping, float angularDamping)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);
		body->setDamping(btScalar(linearDamping), btScalar(angularDamping));
	}
}

void MBulletContext::setObjectTransform(ObjectId objectId, const MVector3 & position, const MQuaternion & rotation)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));
	transform.setRotation(btQuaternion(rotation.values[0], rotation.values[1], rotation.values[2], rotation.values[3]));
	
	btCollisionObject * object = (btCollisionObject *)objectId;
	
	
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);
		if(body->getMotionState())
		{
			if(object->isStaticOrKinematicObject())
			{
				if(object->isKinematicObject())
				{
					m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(
						object->getBroadphaseHandle(),
						m_dynamicsWorld->getDispatcher()
					);
					body->activate(true);
				}
			}
			else
			{
				body->activate(true);
			}
			
			btMotionState * motionState = body->getMotionState();
			motionState->setWorldTransform(transform);
			body->proceedToTransform(transform);
		}
	}
	else if(object->getInternalType() == btCollisionObject::CO_GHOST_OBJECT)
	{
		m_dynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(
			object->getBroadphaseHandle(),
			m_dynamicsWorld->getDispatcher()
		);

		btGhostObject * ghost = btGhostObject::upcast(object);
		
		ghost->setWorldTransform(transform);
		ghost->activate(true);
	}
}

void MBulletContext::getObjectTransform(ObjectId objectId, MVector3 * position, MQuaternion * rotation)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);
		if(body->getMotionState())
		{
			btTransform transform;
			btMotionState * motionState = body->getMotionState();
			motionState->getWorldTransform(transform);
			
			btVector3 tPos = transform.getOrigin();
			btQuaternion tRot = transform.getRotation();

			position->x = tPos.getX();
			position->y = tPos.getY();
			position->z = tPos.getZ();

			rotation->values[0] = tRot.getX();
			rotation->values[1] = tRot.getY();
			rotation->values[2] = tRot.getZ();
			rotation->values[3] = tRot.getW();
		}
	}
}

void MBulletContext::setUserPointer(ObjectId objectId, void *userPointer)
{
	btCollisionObject *object = (btCollisionObject *)objectId;
	object->setUserPointer(userPointer);
}

void * MBulletContext::getUserPointer(ObjectId objectId)
{
	btCollisionObject *object = (btCollisionObject *)objectId;
	return object->getUserPointer();
}

// affectors
void MBulletContext::addCentralForce(ObjectId objectId, const MVector3 & force)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);
		body->activate(true);
		body->applyCentralForce(btVector3(force.x, force.y, force.z));
	}
}

void MBulletContext::getCentralForce(ObjectId objectId, MVector3 * force)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);

		btVector3 totalForce = body->getTotalForce();
		force->x = totalForce.getX();
		force->y = totalForce.getY();
		force->z = totalForce.getZ();
	}
}

void MBulletContext::addTorque(ObjectId objectId, const MVector3 & torque)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);
		body->activate(true);
		body->applyTorque(btVector3(torque.x, torque.y, torque.z));
	}
}

void MBulletContext::getTorque(ObjectId objectId, MVector3 * torque)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);

		btVector3 totalTorque = body->getTotalTorque();
		torque->x = totalTorque.getX();
		torque->y = totalTorque.getY();
		torque->z = totalTorque.getZ();
	}
}

void MBulletContext::clearForces(ObjectId objectId)
{
	btCollisionObject * object = (btCollisionObject *)objectId;
	if(object->getInternalType() == btCollisionObject::CO_RIGID_BODY)
	{
		btRigidBody * body = btRigidBody::upcast(object);
		body->clearForces();
		body->setLinearVelocity(btVector3(0, 0, 0));
		body->setAngularVelocity(btVector3(0, 0, 0));
	}
}

// objects collision
int MBulletContext::isObjectInCollision(ObjectId objectId)
{
	int nbColl = 0;
	btCollisionObject * object = (btCollisionObject *)objectId;

	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for(int i=0; i<numManifolds; i++)
	{
		btPersistentManifold * contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject * obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject * obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

		if((object == obA) || (object == obB))
		{
			int numContacts = contactManifold->getNumContacts();
			for(int j=0; j<numContacts; j++)
			{
				btManifoldPoint & pt = contactManifold->getContactPoint(j);
				if(pt.getDistance() < 0.0f)
				{
					nbColl++;
					break;
				}
			}
		}
	}

	return nbColl;
}

bool MBulletContext::isObjectsCollision(ObjectId object1Id, ObjectId object2Id)
{
	btCollisionObject * object1 = (btCollisionObject *)object1Id;
	btCollisionObject * object2 = (btCollisionObject *)object2Id;
	

	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for(int i=0; i<numManifolds; i++)
	{
		btPersistentManifold * contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject * obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject * obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

		if((object1 == obA && object2 == obB) || (object1 == obB && object2 == obA))
		{
			int numContacts = contactManifold->getNumContacts();
			for(int j=0; j<numContacts; j++)
			{
				btManifoldPoint & pt = contactManifold->getContactPoint(j);
				if(pt.getDistance() < 0.0f)
					return true;
			}
		}
	}

	return false;
}

void MBulletContext::forEachObjectColliding(objectCollidingCallback callback) {
	int numManifolds = m_dynamicsWorld->getDispatcher()->getNumManifolds();
	for(int i=0; i<numManifolds; i++)
	{
		btPersistentManifold * contactManifold = m_dynamicsWorld->getDispatcher()->getManifoldByIndexInternal(i);
		btCollisionObject * obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
		btCollisionObject * obB = static_cast<btCollisionObject*>(contactManifold->getBody1());

		callback(obA, obB);
	}  
}


// create shape
void MBulletContext::createMultiShape(ShapeId * shapeId)
{
	btCompoundShape * shape = new btCompoundShape();
	m_collisionShapes.insert(shape);
	*shapeId = shape;
}

void MBulletContext::createBoxShape(ShapeId * shapeId, const MVector3 & scale)
{
	btCollisionShape * shape = new btBoxShape(btVector3(scale.x, scale.y, scale.z));
	m_collisionShapes.insert(shape);
	*shapeId = shape;
}

void MBulletContext::createSphereShape(ShapeId * shapeId, float radius)
{
	btCollisionShape * shape = new btSphereShape(btScalar(radius));
	m_collisionShapes.insert(shape);
	*shapeId = shape;
}

void MBulletContext::createConeShape(ShapeId * shapeId, float radius, float height)
{
	btCollisionShape * shape = new btConeShape(btScalar(radius), btScalar(height));
	m_collisionShapes.insert(shape);
	*shapeId = shape;
}

void MBulletContext::createCapsuleShape(ShapeId * shapeId, float radius, float height)
{
	btCollisionShape * shape = new btCapsuleShape(btScalar(radius), btScalar(height));
	m_collisionShapes.insert(shape);
	*shapeId = shape;
}

void MBulletContext::createCylinderShape(ShapeId * shapeId, float radius, float height)
{
	btCollisionShape * shape = new btCylinderShape(btVector3(radius, height*0.5f, radius));
	m_collisionShapes.insert(shape);
	*shapeId = shape;
}

void MBulletContext::createConvexHullShape(ShapeId * shapeId, const MVector3 * vertices, unsigned int verticesNumber, const MVector3 scale)
{
	btConvexHullShape * shape = new btConvexHullShape((btScalar*)vertices, verticesNumber, sizeof(MVector3));
	shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	m_collisionShapes.insert(shape);
	*shapeId = shape;
}

void MBulletContext::createTriangleMeshShape(ShapeId * shapeId, const MVector3 * vertices, unsigned int verticesNumber, const void * indices, unsigned int indicesNumber, M_TYPES indicesType, const MVector3 scale)
{
	PHY_ScalarType iType = PHY_INTEGER;
	int triIndexStride = sizeof(int)*3;
	
	if(indicesType == M_USHORT)
	{
		iType = PHY_SHORT;
		triIndexStride = sizeof(short)*3;
	}

	btIndexedMesh iMesh;
	iMesh.m_numTriangles = (int)(indicesNumber/3);
	iMesh.m_numVertices = (int)verticesNumber;
	iMesh.m_vertexStride = sizeof(MVector3);
	iMesh.m_triangleIndexStride = triIndexStride;
	iMesh.m_triangleIndexBase = (const unsigned char *)indices;
	iMesh.m_vertexBase = (const unsigned char *)vertices;

	btTriangleIndexVertexArray * triMesh = new btTriangleIndexVertexArray();
	triMesh->addIndexedMesh(iMesh, iType);

	btBvhTriangleMeshShape * shape = new btBvhTriangleMeshShape(triMesh, true);
	shape->setLocalScaling(btVector3(scale.x, scale.y, scale.z));
	m_collisionShapes.insert(shape);
	*shapeId = shape;
}

// delete shape
void MBulletContext::deleteShape(ShapeId * shapeId)
{
	btCollisionShape * shape = (btCollisionShape *)*shapeId;
	if(shape)
	{
		SAFE_DELETE(shape);
		*shapeId = 0;
	}
}

// add child shape to multishape
void MBulletContext::addChildShape(ShapeId multiShapeId, ShapeId shapeId, const MVector3 & position, const MQuaternion & rotation)
{
	btTransform transform;
	transform.setIdentity();
	transform.setOrigin(btVector3(position.x, position.y, position.z));
	transform.setRotation(
		btQuaternion(rotation.values[0], rotation.values[1], rotation.values[2], rotation.values[3])
		);

	btCompoundShape * multiShape = (btCompoundShape *)multiShapeId;
	btCollisionShape * shape = (btCollisionShape *)shapeId;
	multiShape->addChildShape(transform, shape);
}

// create constraint
void MBulletContext::createConstraint(ConstraintId * constraintId, ObjectId parentObjectId, ObjectId objectId, const MVector3 & pivot, bool disableParentCollision)
{
	btRigidBody * bA = (btRigidBody *)parentObjectId;
	btRigidBody * bB = (btRigidBody *)objectId;
		
	if(bA && bB)
	{
		bA->setActivationState(DISABLE_DEACTIVATION);
		bB->setActivationState(DISABLE_DEACTIVATION);

		MVector3 position, euler;
		MQuaternion rotation;
		MMatrix4x4 matrix, matrix1, matrix2, rotMatrix;
		
		rotMatrix.setRotationEuler(90, 0, 0);
		
		getObjectTransform(parentObjectId, &position, &rotation);
		matrix1.setRotationAxis(rotation.getAngle(), rotation.getAxis());
		matrix1.setTranslationPart(position);
		
		getObjectTransform(objectId, &position, &rotation);
		matrix2.setRotationAxis(rotation.getAngle(), rotation.getAxis());
		matrix2.setTranslationPart(position + matrix2.getRotatedVector3(pivot));
		
		matrix = (matrix1.getInverse() * matrix2) * rotMatrix;
		
		euler = matrix.getEulerAngles();
		position = matrix.getTranslationPart();
		rotation.setFromAngles(euler.x, euler.y, euler.z);
		
		btTransform frameInA, frameInB;
		frameInA = btTransform::getIdentity();
		frameInA.setOrigin(btVector3(position.x, position.y, position.z));
		frameInA.setRotation(btQuaternion(rotation.values[0], rotation.values[1], rotation.values[2], rotation.values[3]));
		
		rotation.setFromAngles(90, 0, 0);
		frameInB = btTransform::getIdentity();
		frameInB.setOrigin(btVector3(pivot.x, pivot.y, pivot.z));
		frameInB.setRotation(btQuaternion(rotation.values[0], rotation.values[1], rotation.values[2], rotation.values[3]));
			
		btGeneric6DofSpringConstraint * constraint = new btGeneric6DofSpringConstraint(
			*bA, *bB, frameInA, frameInB, true
		);
		
		m_dynamicsWorld->addConstraint(constraint, disableParentCollision);
		*constraintId = constraint;
	}
}

void MBulletContext::setLinearLimit(ConstraintId constraintId, const MVector3 & lower, const MVector3 & upper)
{
	btGeneric6DofSpringConstraint * constraint = (btGeneric6DofSpringConstraint *)constraintId;
	if(constraint)
	{
		constraint->setLinearLowerLimit(btVector3(lower.x, lower.z, -upper.y));
		constraint->setLinearUpperLimit(btVector3(upper.x, upper.z, -lower.y));
	}
}

void MBulletContext::setAngularLimit(ConstraintId constraintId, const MVector3 & lower, const MVector3 & upper)
{
	btGeneric6DofSpringConstraint * constraint = (btGeneric6DofSpringConstraint *)constraintId;
	if(constraint)
	{
		constraint->setAngularLowerLimit(btVector3(btScalar(lower.x*DEG_TO_RAD), btScalar(lower.z*DEG_TO_RAD), btScalar(-upper.y*DEG_TO_RAD)));
		constraint->setAngularUpperLimit(btVector3(btScalar(upper.x*DEG_TO_RAD), btScalar(upper.z*DEG_TO_RAD), btScalar(-lower.y*DEG_TO_RAD)));
	}
}

// delete constraint
void MBulletContext::deleteConstraint(ConstraintId * constraintId)
{
	btTypedConstraint * constraint = (btTypedConstraint *)*constraintId;
	if(constraint)
	{
		m_dynamicsWorld->removeConstraint(constraint);
		SAFE_DELETE(constraint);
		*constraintId = 0;
	}
}
