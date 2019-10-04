#define GLUT_DISABLE_ATEXIT_HACK
#include <bits/stdc++.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <cstdlib>
#include <time.h>
#include <GL/glut.h>
#include "boids.h"

using namespace std;

RandNumbers temp;

std::vector<Boid*> boids;
std::vector<Objective*> objectives = {};

void Boid::move_up(){
	if (pt->y >= 300) pt->y -= boid_speed;
	else pt->y += boid_speed;
}
	
void Boid::move_left(){
	if (pt->x <= -300) pt->x += boid_speed;
	else pt->x -= boid_speed;
}	
	
void Boid::move_down(){
	if (pt->y <= -300) pt->y += boid_speed;
	else pt->y -= boid_speed;
}
	
void Boid::move_right(){
	if (pt->x >= 300) pt->x -= boid_speed;
	else pt->x += boid_speed;
}
	
void Boid::move_towards_center(){
	center_x = 0;
	center_y = 0;
	for(int i = 0; i < boids.size(); i++){
		if (boids[i]->get_pt()->x == pt->x 
			&& boids[i]->get_pt()->y == pt->y) continue;
		center_x += boids[i]->get_pt()->x;
		center_y += boids[i]->get_pt()->y;
	}
		
	center_x /= (boids.size() - 1);
	center_y /= (boids.size() - 1);
}
	
void Boid::avoid_boids(){
	distance_x = 0;
	distance_y = 0;
	for(int i = 0; i < boids.size(); i++){
		if (boids[i]->get_pt()->x == pt->x 
			&& boids[i]->get_pt()->y == pt->y) continue;
		float dist = distance(boids[i]->get_pt(), this->pt);
		if (dist <= 10.0f){
			random_move();
		}
	}
}

void Boid::move_towards_objective(){
	objective_x = 0; objective_y = 0;
	if (objectives.empty()) return;
	Objective* obj = objectives[0];
	if (pt->x <= obj->pt->x){
		objective_x += boid_speed;
	} else {
		objective_x -= boid_speed;
	}
	
	if (pt->y <= obj->pt->y){
		objective_y += boid_speed;
	} else {
		objective_y -= boid_speed;
	}
}
	
void Boid::random_move(){
		
	if (cur_moves >= max_moves){
		moved = false;
	}
	
	if (!moved){
		srand(temp.rdtsc());
		dir = rand() % 4;
		cur_moves = 0;
		moved = true;
	} else {
		cur_moves++;
	}
	
	if (dir == 0){ //move up
		move_up();
	} else if (dir == 1){ //move left
		move_left();
	} else if (dir == 2){ //move down
		move_down();
	} else { //move right
		move_right();
	}
}
	
void Boid::set_pt(Point2D* pt) { this->pt = pt; }
	
Point2D* Boid::get_pt() { return this->pt; }
	
void Boid::move(){
	move_towards_center();
	if (pt->y <= center_y) move_up();
	else if (pt->y >= center_y) move_down();
	
	if (pt->x >= center_x) move_left();
	else if (pt->x <= center_x) move_right();
	
	avoid_boids();
	
	pt->x += distance_x;
	pt->y += distance_y;
	
	move_towards_objective();
	
	pt->x += objective_x;
	pt->y += objective_y;
	
	if (objectives.empty()) return;
	if (distance(objectives[0]->pt, this->pt) <= 2.0f){
		objectives.erase(objectives.begin() + 0);
	}
	
	//random_move();
}
