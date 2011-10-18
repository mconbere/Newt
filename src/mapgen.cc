// Copyright (c) 2011 Brett McLarnon. All rights reserved.
// Author: Brett McLarnon <brett.mclarnon@gmail.com>

#include "mapgen.h"

#include <cstdlib>
#include <list>
#include <map>
#include <set>
#include <utility>

#include "MEngine.h"
#include "MWindow.h"

namespace newt {
namespace {

// generates a random number in the range [0, max)
int Random(int max) {
  int r;
  int bound = (RAND_MAX/max) * max;
  while ((r = rand()) >= bound);
  return r % max;
}

struct Room {
  int doors[4]; // west, south, north, east.  -1 is a wall
};

// adds the doors of a room in a random order.
void AddDoors(const Room input[], const int mapping[], int room,
              std::list<int>* edges) {
  // Generate a random permutation of [0 1 2 3].
  int order[4];
  order[0] = 0;
  for (int n = 1; n < 4; ++n) {
    int j = Random(n + 1);
    order[n] = order[j];
    order[j] = n;
  }

  for (int i = 0; i < 4; ++i) {
    int direction = order[i];
    int dst = input[room].doors[direction];
    if (dst != -1 && mapping[dst] == -1)
      edges->push_front(room*4 + direction);
  }
}

void GenerateRooms(const Room input[], int insize, int outsize, Room output[]) {
  static const int skip = 60; // percentage chance of skipping an edge
  int mapping[insize];
  std::list<int> edges;
  
  if (insize < outsize) {
    return;
  }
  
  memset(mapping, -1, sizeof(mapping));
  memset(output, -1, outsize*sizeof(Room));
  
  // Pick a random starting room and add it to our map.
  int start = Random(insize);
  mapping[start] = 0;
  AddDoors(input, mapping, start, &edges);
  
  // Keep adding edges until we've generated a sufficiently large map.
  int count = 1;
  while (count < outsize) {
    if (edges.empty())
      return;
    
    int edge = edges.front();
    edges.pop_front();
    
    // skip % of the time, move the edge to the back of the queue.
    // the rest of the time, add the edge to our map.
    if (Random(100) < skip) {
      edges.push_back(edge);
    } else {
      int src = edge/4;
      int direction = edge%4;
      int dst = input[src].doors[direction];
      
      // If we haven't already seen the destination room, add it to the
      // mapping and add its doors to the edges list.
      if (mapping[dst] == -1) {
        mapping[dst] = count++;
        AddDoors(input, mapping, dst, &edges);
      }
      
      // Add the edge to our map.
      output[mapping[src]].doors[direction] = mapping[dst];
      output[mapping[dst]].doors[direction^3] = mapping[src];
    }
  }
}

void CalculatePositions(const Room map[], int room,
                        const std::pair<int, int>& pos,
                        std::set<int>* seen,
                        std::map<std::pair<int, int>, int>* positions) {
  if (seen->find(room) != seen->end())
    return;
  
  // Update traversal state
  seen->insert(room);
  (*positions)[pos] = room;
  
  // Recurse
  if (map[room].doors[0] != -1)
    CalculatePositions(map, map[room].doors[0],
                       make_pair(pos.first-1, pos.second), seen, positions);
  if (map[room].doors[1] != -1)
    CalculatePositions(map, map[room].doors[1],
                       make_pair(pos.first, pos.second-1), seen, positions);
  if (map[room].doors[2] != -1)
    CalculatePositions(map, map[room].doors[2],
                       make_pair(pos.first, pos.second+1), seen, positions);
  if (map[room].doors[3] != -1)
    CalculatePositions(map, map[room].doors[3],
                       make_pair(pos.first+1, pos.second), seen, positions);
}

void CreateRoom(const std::pair<int, int>& location, int number,
                MMeshRef* room_mesh, MScene* scene) {
  char name[10];
  snprintf(name, sizeof(name), "room.%d", number);
  
  MOEntity* entity =
  scene->addNewEntity(room_mesh, std::map<std::string, std::string>());
  entity->setName(name);
  entity->setActive(true);
  entity->setInvisible(false);
  *entity->getBoundingBox() = *room_mesh->getMesh()->getBoundingBox();
  entity->setPosition(MVector3(location.first * 100.0,
                               location.second * 100.0, 0.0));
  entity->setScale(MVector3(1.0, 1.0, 1.0));
  MPhysicsProperties* physics_props = entity->createPhysicsProperties();
  physics_props->setCollisionShape(M_COLLISION_SHAPE_TRIANGLE_MESH);
}

void CreateRooms(const Room *map, MMeshRef* room_mesh, MScene* scene) {
  std::set<int> seen;
  std::map<std::pair<int, int>, int> positions;
  CalculatePositions(map, 0, make_pair(0, 0), &seen, &positions);

  for (std::map<std::pair<int, int>, int>::const_iterator it =
       positions.begin(); it != positions.end(); ++it) {
    CreateRoom(it->first, it->second, room_mesh, scene);
    // TODO(bmclarnon): Add door meshes as well.
  }
}

}  // namespace

bool MapGen::CreateScene(MScene* scene) {
  /*
   0--1--2
   |     |
   3  4--5--6
   |     |
   7--8--9--10
   |
   11
   */
  struct Room fixed[] = {
    {-1, 3, -1, 1}, // 0
    {0, -1, -1, 2}, // 1
    {1, 5, -1, -1}, // 2
    {-1, -1, 0, -1}, // 3
    {-1, 8, -1, 5}, // 4
    {4, -1, 2, 6}, // 5
    {5, 10, -1, -1}, // 6
    {-1, -1, -1, 8}, // 7
    {7, -1, 4, 9}, // 8
    {8, -1, -1, 10}, // 9
    {9, 11, 6, -1}, // 10
    {-1, -1, 10, -1}, // 11
  };
  
  srand(time(NULL));
  Room map[10];
  GenerateRooms(fixed, sizeof(fixed)/sizeof(Room), sizeof(map)/sizeof(Room),
                map);
  
  CreateRooms(map, room_mesh_, scene);
  
  return true;
}
  
}  // namespace newt
