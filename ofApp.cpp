#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(60);
	ofSetWindowTitle("openFrameworks");

	ofBackground(239);
	ofSetLineWidth(2);

	this->box2d.init();
	this->box2d.setGravity(0, 30);
	this->box2d.createBounds();
	this->box2d.setFPS(60);

	this->base_color_list.push_back(ofColor(39));

	for (int i = 0; i < 2; i++) {

		this->noise_param_list.push_back(glm::vec2(ofRandom(1000), ofRandom(1000)));
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	if(ofGetFrameNum() % 10 == 0) {
		
		for (int i = 0; i < this->noise_param_list.size(); i++) {

			auto location = glm::vec2(
				ofMap(ofNoise(this->noise_param_list[i].x, ofGetFrameNum() * 0.008), 0, 1, 100, 620),
				ofMap(ofNoise(this->noise_param_list[i].y, ofGetFrameNum() * 0.008), 0, 1, 100, 360));
			auto target = glm::vec2(
				ofMap(ofNoise(this->noise_param_list[i].x, (ofGetFrameNum() + 1) * 0.008), 0, 1, 100, 620),
				ofMap(ofNoise(this->noise_param_list[i].y, (ofGetFrameNum() + 1) * 0.008), 0, 1, 100, 360));

			auto size = 30;
			auto angle = std::atan2(target.y - location.y, target.x - location.x);

			int color_index = ofRandom(this->base_color_list.size());

			vector<glm::vec2> vertices;
			vertices.push_back(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25);
			vertices.push_back(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)));
			vertices.push_back(location + glm::vec2(size * cos(angle), size * sin(angle)));
			vertices.push_back(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)));
			vertices.push_back(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25);

			vertices.push_back(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25);
			vertices.push_back(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5);
			vertices.push_back(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5);
			vertices.push_back(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25);

			auto polygon = make_shared<ofxBox2dPolygon>();
			polygon->addTriangle(vertices[0], vertices[1], vertices[2]);
			polygon->setPhysics(1.0, 0.7, 0.3);
			polygon->create(box2d.getWorld());
			
			this->polygon_list.push_back(move(polygon));
			this->polygon_life_list.push_back(128);
			this->polygon_color_list.push_back(this->base_color_list[color_index]);

			polygon = make_shared<ofxBox2dPolygon>();
			polygon->addTriangle(vertices[0], vertices[2], vertices[4]);
			polygon->setPhysics(1.0, 0.7, 0.3);
			polygon->create(box2d.getWorld());

			this->polygon_list.push_back(move(polygon));
			this->polygon_life_list.push_back(128);
			this->polygon_color_list.push_back(this->base_color_list[color_index]);

			polygon = make_shared<ofxBox2dPolygon>();
			polygon->addTriangle(vertices[4], vertices[2], vertices[3]);
			polygon->setPhysics(1.0, 0.7, 0.3);
			polygon->create(box2d.getWorld());

			this->polygon_list.push_back(move(polygon));
			this->polygon_life_list.push_back(128);
			this->polygon_color_list.push_back(this->base_color_list[color_index]);

			polygon = make_shared<ofxBox2dPolygon>();
			polygon->addTriangle(vertices[5], vertices[6], vertices[7]);
			polygon->setPhysics(1.0, 0.7, 0.3);
			polygon->create(box2d.getWorld());

			this->polygon_list.push_back(move(polygon));
			this->polygon_life_list.push_back(128);
			this->polygon_color_list.push_back(this->base_color_list[color_index]);
			
			polygon = make_shared<ofxBox2dPolygon>();
			polygon->addTriangle(vertices[5], vertices[7], vertices[8]);
			polygon->setPhysics(1.0, 0.7, 0.3);
			polygon->create(box2d.getWorld());

			this->polygon_list.push_back(move(polygon));
			this->polygon_life_list.push_back(128);
			this->polygon_color_list.push_back(this->base_color_list[color_index]);
		}
	}

	for (int i = this->polygon_list.size() - 1; i > -1; i--) {

		this->polygon_life_list[i] -= 1;

		if (this->polygon_life_list[i] < 0) {

			this->polygon_list[i]->destroy();
			this->polygon_list.erase(this->polygon_list.begin() + i);
			this->polygon_life_list.erase(this->polygon_life_list.begin() + i);
			this->polygon_color_list.erase(this->polygon_color_list.begin() + i);
		}
	}

	this->box2d.update();
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofNoFill();
	ofSetColor(39);

	for (int i = 0; i < this->noise_param_list.size(); i++) {

		auto location = glm::vec2(
			ofMap(ofNoise(this->noise_param_list[i].x, ofGetFrameNum() * 0.008), 0, 1, 100, 620),
			ofMap(ofNoise(this->noise_param_list[i].y, ofGetFrameNum() * 0.008), 0, 1, 100, 360));
		auto target = glm::vec2(
			ofMap(ofNoise(this->noise_param_list[i].x, (ofGetFrameNum() + 1) * 0.008), 0, 1, 100, 620),
			ofMap(ofNoise(this->noise_param_list[i].y, (ofGetFrameNum() + 1) * 0.008), 0, 1, 100, 360));

		auto size = 30;
		auto angle = std::atan2(target.y - location.y, target.x - location.x);

		ofSetColor(39);
		ofNoFill();
		ofBeginShape();
		ofVertex(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25);
		ofVertex(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)));
		ofVertex(location + glm::vec2(size * cos(angle), size * sin(angle)));
		ofVertex(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)));
		ofVertex(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25);
		ofEndShape();

		ofBeginShape();
		ofVertex(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25);
		ofVertex(location + glm::vec2(size * 0.5 * cos(angle - PI * 0.5), size * 0.5 * sin(angle - PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5);
		ofVertex(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25 - glm::vec2(size * cos(angle), size * sin(angle)) * 0.5);
		ofVertex(location + glm::vec2(size * 0.5 * cos(angle + PI * 0.5), size * 0.5 * sin(angle + PI * 0.5)) * 0.25);
		ofEndShape();
	}

	ofFill();
	for (int i = 0; i < this->polygon_list.size(); i++) {

		if (this->polygon_life_list[i] > 64) {

			ofSetColor(this->polygon_color_list[i]);
		}
		else {

			ofSetColor(this->polygon_color_list[i], ofMap(this->polygon_life_list[i], 64, 0, 255, 0));
		}

		this->polygon_list[i]->draw();
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}