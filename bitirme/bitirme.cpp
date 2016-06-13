#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>
#include <sstream>

#include <vector>
//#include <algorithm>
#include <iostream>
#include <stdio.h>
#include <math.h>
#define RADTODEG 57.295779513082320876f

static const float SCALE = 25;

using namespace std;

class node{
public:
	b2Vec2 coordinate;
	vector<node*> next;
	int id;
	//dijkstra
	int distance;
	int prev;
	bool inlist;
	node(b2Vec2 coor,int sira){
		coordinate=coor;
		id=sira;
		distance=100000;
		prev=-1;
		inlist = 1;
	}
	
	float find_distance(node diger){
		return sqrt(pow((coordinate.x-diger.coordinate.x),2)+pow((coordinate.y-diger.coordinate.y),2));
	}
	
};

class siralama {
public:
	int mesafe;
	int id;
	siralama(int mes, int i) { mesafe = mes; id = i; }
};

b2Body *robot, *object1, *obstacleobje;
b2World World(b2Vec2(0, 0));



int main() {
	printf("type(1 my algorithm- 2 prm):"); char secimyap;
	cin >> secimyap;
	printf("shape(1 star- 2 u- 3 z):"); char sekilal;
	cin >> sekilal;

	long elapsedtime;
	clock_t baslangiczamani, bitiszamani;
		//Box2D
	b2Vec2 robotbaslangic, objebaslangic, obstacle;
	if (sekilal == '1')
	{
		robotbaslangic = b2Vec2(500, 550);//500 550
		objebaslangic = b2Vec2(350, 100);//350 100
	}
	else if (sekilal == '2')
	{
		robotbaslangic=b2Vec2(450, 300);//500 550
		objebaslangic=b2Vec2(750, 300);//350 100
		
	}
	else if(sekilal =='3') {
		robotbaslangic = b2Vec2(700, 300);//500 550
		objebaslangic = b2Vec2(100, 300);//350 100
	}
	obstacle = b2Vec2(400, 300);
	//robot
	b2BodyDef myBodyDef;
	myBodyDef.type = b2_dynamicBody;
	//myBodyDef.position.Set(coordinate[source].x/SCALE,coordinate[source].y/SCALE);
	myBodyDef.position.Set(robotbaslangic.x / SCALE, robotbaslangic.y / SCALE);

	robot = World.CreateBody(&myBodyDef);
	b2CircleShape circle;
	circle.m_p.Set(0, 0);
	circle.m_radius = 25 / SCALE;
	b2FixtureDef myFixtureDef;
	myFixtureDef.shape = &circle;
	myFixtureDef.density = 5;
	myFixtureDef.friction = 3;
	myFixtureDef.restitution = 0.3;
	robot->CreateFixture(&myFixtureDef);

	//object1
	myBodyDef.position.Set(objebaslangic.x / SCALE, objebaslangic.y / SCALE);
	object1 = World.CreateBody(&myBodyDef);
	b2PolygonShape box;
	box.SetAsBox(25 / SCALE, 25 / SCALE);
	myFixtureDef.shape = &box;
	myFixtureDef.density = 1;
	object1->CreateFixture(&myFixtureDef);
	//obstacles

	myBodyDef.position.Set(obstacle.x / SCALE, obstacle.y / SCALE);
	myBodyDef.type = b2_staticBody;
	obstacleobje = World.CreateBody(&myBodyDef);

	b2ChainShape chainShape;
	b2Vec2 vertices[9];
	if (sekilal == '1')
	{
		vertices[0].Set(0 / SCALE, 128 / SCALE);
		vertices[1].Set(-20 / SCALE, 20 / SCALE);
		vertices[2].Set(-256 / SCALE, 0 / SCALE);
		vertices[3].Set(-20 / SCALE, -20 / SCALE);
		vertices[4].Set(0 / SCALE, -128 / SCALE);
		vertices[5].Set(20 / SCALE, -20 / SCALE);
		vertices[6].Set(256 / SCALE, 0 / SCALE);
		vertices[7].Set(20 / SCALE, 20 / SCALE);
		vertices[8].Set(0 / SCALE, 128 / SCALE);
	}
	else if (sekilal == '2')
	{
		vertices[0].Set(-200 / SCALE, 170 / SCALE);
		vertices[1].Set(-200 / SCALE, 150 / SCALE);
		vertices[2].Set(200 / SCALE, 150 / SCALE);
		vertices[3].Set(200 / SCALE, -150 / SCALE);
		vertices[4].Set(-200 / SCALE, -150 / SCALE);
		vertices[5].Set(-200 / SCALE, -170 / SCALE);
		vertices[6].Set(220 / SCALE, -170 / SCALE);
		vertices[7].Set(220 / SCALE, 170 / SCALE);
		vertices[8].Set(-200 / SCALE, 170 / SCALE);
	}
	else if (sekilal == '3')
	{
		vertices[0].Set(120 / SCALE, 100 / SCALE);
		vertices[1].Set(120 / SCALE, 200 / SCALE);
		vertices[2].Set(100 / SCALE, 200 / SCALE);
		vertices[3].Set(100 / SCALE, 100 / SCALE);
		vertices[4].Set(-120 / SCALE, -100 / SCALE);
		vertices[5].Set(-120 / SCALE, -200 / SCALE);
		vertices[6].Set(-100 / SCALE, -200 / SCALE);
		vertices[7].Set(-100 / SCALE, -100 / SCALE);
		vertices[8].Set(120 / SCALE, 100 / SCALE);
	}
	chainShape.CreateChain(&vertices[0], 9);
	myFixtureDef.shape = &chainShape;
	obstacleobje->CreateFixture(&myFixtureDef);
	
	//////////////////////////////////////////////////////////////
	printf("creating graph...\n");
	vector<node> motiongraph;
	
	
	if (secimyap == '1') {
		baslangiczamani = clock();
		printf("My Algorithm:\n");
		srand(time(NULL));

		b2Vec2 eskihiz(-2, -2);
		b2Vec2 oncekirobotposition = robotbaslangic;
		int id = 0;

		node *ilk = new node(robotbaslangic, id);
		motiongraph.push_back(*ilk);
		id++;
		delete ilk;

		///
		int tutma = -1; bool ilkmi = 1;
		///

		while (1) {



			b2Vec2 hiz((rand() % 3) - 1, (rand() % 3) - 1);


			if (eskihiz.x == hiz.x*-1 && eskihiz.y == hiz.y*-1)
				continue;
			if (hiz.x == 0 && hiz.y == 0)
				continue;

			eskihiz = hiz;

			robot->SetLinearVelocity(hiz);

			World.Step(2, 8, 3);

			b2Vec2 robotposition, objectposition;
			robotposition.x = robot->GetPosition().x*SCALE;  robotposition.y = robot->GetPosition().y*SCALE;
			objectposition.x = object1->GetPosition().x*SCALE;  objectposition.y = object1->GetPosition().y*SCALE;

			if (robotposition.x > 800 || robotposition.x < 0 || robotposition.y < 0 || robotposition.y>600) {
				robot->SetTransform(b2Vec2(oncekirobotposition.x / SCALE, oncekirobotposition.y / SCALE), 0);
				continue;

			}

			bool carpma = 0;
			for (b2ContactEdge* edge = obstacleobje->GetContactList(); edge; edge = edge->next) {
				b2Fixture *a = edge->contact->GetFixtureB();
				if (a->GetBody() == robot)
				{
					robot->SetTransform(b2Vec2(oncekirobotposition.x / SCALE, oncekirobotposition.y / SCALE), 0);
					carpma = 1;
					break;
				}
			}
			if (carpma)continue;
			////////yönü belirle


			/////////graphe ekle

			node *gecici = new node(robotposition,  id);
			motiongraph.push_back(*gecici); id++;
			delete gecici;
			oncekirobotposition = robotposition;

			///////


			//////////
			if (objebaslangic.x != objectposition.x || objebaslangic.y != objectposition.y) {
				break;
			}
		}
		///////
		node *enson = new node(objebaslangic, motiongraph.size());
		motiongraph.push_back(*enson);
		delete enson;
		//add edges	////////////////////////////////////////////////////////////
		printf("adding edges\n");
		for (int i = 1; i < motiongraph.size(); i++)

			motiongraph[i - 1].next.push_back(&motiongraph[i]);



		////50den kucuk nodelari birlestir

		 for (int i = 0; i < motiongraph.size() - 2; i++) {///motiongraph.size()
			for (int j = i + 2; j < motiongraph.size(); j++) {///motiongraph.size()

				if (motiongraph[i].find_distance(motiongraph[j]) <100) {

					motiongraph[i].next.push_back(&motiongraph[j]);

				}

			}
		}
		 bitiszamani = clock();
	}
	///////prm
	else {
		baslangiczamani = clock();
		printf("prm:\n");
		node *baslangicnoktasi = new node(robotbaslangic, 0);
		motiongraph.push_back(*baslangicnoktasi);
		delete baslangicnoktasi;
		printf("prm creating random point\n");
		srand(time(NULL));
		float x, y; int idsi = 1;
		int saybakalim = 0; int tipi = 0;
		while (tipi < 8)
		{
			while (1)
			{
				if (tipi == 0) {
					x = rand() % 200;
					y = rand() % 300;
				}
				else if (tipi == 1) {
					x = rand() % 200 + 200;
					y = rand() % 300;
				}
				else if (tipi == 2) {
					x = rand() % 200+400;
					y = rand() % 300 ;
				}
				else if (tipi == 3) {
					x = rand() % 200 + 600;
					y = rand() % 300 ;
				}
				else if(tipi ==4){
					x = rand() % 200;
					y = rand() % 300 + 300;
				}
				else if(tipi==5){
					x = rand() % 200+200;
					y = rand() % 300 + 300;
				}
				else if(tipi==6){
					x = rand() % 200+400;
					y = rand() % 300 + 300;
				}
				else if(tipi==7){
					x = rand() % 200+600;
					y = rand() % 300 + 300;
				}

				/////////////////
				
				
				robot->SetLinearVelocity(b2Vec2(0, 0));
				object1->SetLinearVelocity(b2Vec2(0, 0));
				object1->SetTransform(b2Vec2(objebaslangic.x / SCALE, objebaslangic.y / SCALE), 0);
				object1->SetAngularVelocity(0);
				robot->SetTransform(b2Vec2(x / SCALE, y / SCALE), 0);
				bool devamet = 0;
				for (b2ContactEdge* edge = robot->GetContactList(); edge; edge = edge->next) {
					b2Fixture *a = edge->contact->GetFixtureA();
					if (a->GetBody() == obstacleobje)
					{
						devamet = 1;
						break;
					}
				}
				if (devamet)continue;
				///////////////////

				node *ekle = new node(b2Vec2(x, y), idsi); idsi++;
				motiongraph.push_back(*ekle);
				break;
			}

			saybakalim++;
			if (saybakalim == 25) {
				tipi++; saybakalim = 0;

			}
		}
		////////////////////////
		
		///////////////////////
		node *yeniekle = new node(objebaslangic, motiongraph.size());
		motiongraph.push_back(*yeniekle);
		delete yeniekle;

		////prm edgeleri ekle
		vector<siralama> mesafeler;
		for (int i = 0; i < motiongraph.size(); i++)
		{
			for (int j = 0; j < motiongraph.size(); j++) {
				siralama *gecici = new siralama(motiongraph[i].find_distance(motiongraph[j]), j);
				mesafeler.push_back(*gecici);
				delete gecici;
			}

			for (int k = 1; k < motiongraph.size(); k++) {
				int a = k;
				while (a > 0 && mesafeler[a - 1].mesafe > mesafeler[a].mesafe) {
					siralama gecicidir = mesafeler[a];
					mesafeler[a] = mesafeler[a - 1];
					mesafeler[a - 1] = gecicidir;
					a = a - 1;
				}
			}
			///////
			
			///////
			for (int y = 1; y <= 100; y++) {
				/////carpma kontrolü box2d ile
				robot->SetLinearVelocity(b2Vec2(0, 0));
				
				object1->SetLinearVelocity(b2Vec2(0, 0));
				object1->SetTransform(b2Vec2(objebaslangic.x / SCALE, objebaslangic.y / SCALE), 0);
				object1->SetAngularVelocity(0);
				robot->SetTransform(b2Vec2(motiongraph[i].coordinate.x / SCALE, motiongraph[i].coordinate.y / SCALE), 0);
				int x_karsilastir, y_karsilastir, ilki = 1; bool ekle = 0, cikmalidir = 0;
				while (1) {
					World.Step(2, 8, 3);
					//printf("%d", i);
					int x_degeri = robot->GetPosition().x*SCALE;
					int y_degeri = robot->GetPosition().y*SCALE;
					if (ilki == 1) {
						robot->SetLinearVelocity(b2Vec2((motiongraph[mesafeler[y].id].coordinate.x - motiongraph[i].coordinate.x) / SCALE, (motiongraph[mesafeler[y].id].coordinate.y - motiongraph[i].coordinate.y) / SCALE));


						x_karsilastir = x_degeri - motiongraph[mesafeler[y].id].coordinate.x;
						y_karsilastir = y_degeri - motiongraph[mesafeler[y].id].coordinate.y;
						ilki = 0;
					}

					for (b2ContactEdge* edge = robot->GetContactList(); edge; edge = edge->next) {
						b2Fixture *a = edge->contact->GetFixtureA();
						if (a->GetBody() == obstacleobje)
						{
							cikmalidir = 1;
							break;
						}
					}
					if (cikmalidir)
						break;

					if (x_karsilastir * (x_degeri - motiongraph[mesafeler[y].id].coordinate.x) < 0 || y_karsilastir * (y_degeri - motiongraph[mesafeler[y].id].coordinate.y) < 0)
					{
						ekle = 1;
						break;
					}

				}
				//////
				if (ekle)
					motiongraph[i].next.push_back(&motiongraph[mesafeler[y].id]);
			}
			vector<siralama>().swap(mesafeler);
		}
		bitiszamani = clock();
    }
	/////////yaz
	for (int i = 0; i < motiongraph.size(); i++) {
		printf("%d ---", motiongraph[i].id);
		for (int j = 0; j < motiongraph[i].next.size(); j++) {
			printf("(%d) ", motiongraph[i].next[j]->id);
			if (j == 10)
				break;
		}
		
		printf("\n");
	}
	
	//printf("%d\n", motiongraph.size());
	system("PAUSE");
	////////////dikkstra//////////////////////////////////////
	
	 printf("\nfinding path...\n");
	 motiongraph[0].distance = 0;
	 while (1) {
		 int minumum = -1, gecici = 100000000;
		 for (int i = 0; i < motiongraph.size(); i++) {
			 if (motiongraph[i].distance < gecici && motiongraph[i].inlist!=0) {
				 minumum = i;
				 gecici = motiongraph[i].distance;
			 }
		 }
		 motiongraph[minumum].inlist = 0;///remove node

		 for (int i = 0; i < motiongraph[minumum].next.size(); i++) {

			 int alt = motiongraph[minumum].distance + motiongraph[minumum].find_distance(*motiongraph[minumum].next[i]);///!
			 
			 if (alt < motiongraph[minumum].next[i]->distance) {
				 motiongraph[minumum].next[i]->distance = alt;
				 motiongraph[minumum].next[i]->prev = minumum;
				 
			 }
		 }




		 bool cik = 1;
		 for (int i = 0; i < motiongraph.size(); i++) {
			 if (motiongraph[i].inlist == 1) {
				 cik = 0;
				 break;
			 }

		 }

		 if (cik)break;
	 }
	 ////yolu alma
	 bool olmadi = 0;
	 vector<int> yol;
	 printf("yol\n");
	 yol.push_back(motiongraph.size()-1);
	 node gecici = motiongraph[motiongraph.size()-1];
	 while (1) {
		 //printf("%d %0.f %0.f\n", gecici.prev,gecici.coordinate.x,gecici.coordinate.y);
		 if (gecici.prev == -1) {
			 olmadi = 1;
			 break;
		 }
		 yol.push_back(gecici.prev);
		 gecici = motiongraph[gecici.prev];
		 if (gecici.prev == 0) {
			 yol.push_back(0);
			 break;
		 }
	 }
	 
	 if (olmadi) {
		 printf("no path\n");
		 system("PAUSE");
		 return 0;
	 }
	 
		 std::reverse(yol.begin(), yol.end());
		 for (int i = 0; i < yol.size(); i++)
			 printf("%d %d %0.f %0.f\n", i, yol[i], motiongraph[yol[i]].coordinate.x, motiongraph[yol[i]].coordinate.y);

		 int totaldistance = 0;
		 for (int i = 0; i < yol.size() - 1; i++) {
			 totaldistance += motiongraph[yol[i]].find_distance(motiongraph[yol[i + 1]]);
			 printf("%0.f\n", motiongraph[yol[i]].find_distance(motiongraph[yol[i + 1]]));
		 }

		 printf("total distance:%d\n", totaldistance);
		 elapsedtime = ((double)bitiszamani - baslangiczamani) / CLOCKS_PER_SEC * 1000;
		 printf("elapsed time:%ld ms\n", elapsedtime);
		 /////return inintial positions
		 robot->SetLinearVelocity(b2Vec2(0, 0));
		 robot->SetTransform(b2Vec2(robotbaslangic.x / SCALE, robotbaslangic.y / SCALE), 0);
		 object1->SetLinearVelocity(b2Vec2(0, 0));
		 object1->SetTransform(b2Vec2(objebaslangic.x / SCALE, objebaslangic.y / SCALE), 0);
		 object1->SetAngularVelocity(0);



		 //////
		 int say = 0;
		 int x_karsilastir;
		 int y_karsilastir;
		 bool ilki = 1;

		 //sfml

		 printf("\nsimulation is starting\n");
		 system("PAUSE");
		 sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Simulation");
		 Window.setFramerateLimit(60);

		 int carp = 0;

		 while (Window.isOpen()) {

			 sf::Event event;
			 while (Window.pollEvent(event))
			 {

				 if (event.type == sf::Event::Closed)
					 Window.close();

				 if (event.type == sf::Event::KeyPressed) {
					 if (event.key.code == sf::Keyboard::D)
						 robot->SetLinearVelocity(b2Vec2(5, 0));
					 else if (event.key.code == sf::Keyboard::A)
						 robot->SetLinearVelocity(b2Vec2(-5, 0));
					 else if (event.key.code == sf::Keyboard::W)
						 robot->SetLinearVelocity(b2Vec2(0, -5));
					 else if (event.key.code == sf::Keyboard::S)
						 robot->SetLinearVelocity(b2Vec2(0, 5));
				 }
			 }



			 Window.clear(sf::Color::White);
			 World.Step(1 / 30.f, 8, 3);

			 sf::CircleShape robot_shape(25);
			 robot_shape.setOrigin(25, 25);
			 robot_shape.setPosition(robot->GetPosition().x*SCALE, robot->GetPosition().y*SCALE);
			 robot_shape.setFillColor(sf::Color::Blue);
			 robot_shape.setRotation(robot->GetAngle() * 180 / b2_pi);
			 Window.draw(robot_shape);

			 sf::RectangleShape object1_shape(sf::Vector2f(50, 50));
			 object1_shape.setOrigin(25, 25);
			 object1_shape.setPosition(object1->GetPosition().x*SCALE, object1->GetPosition().y*SCALE);
			 object1_shape.setFillColor(sf::Color::Red);
			 object1_shape.setRotation(object1->GetAngle() * 180 / b2_pi);
			 Window.draw(object1_shape);


			 //obstacle
			 if (sekilal == '1')
			 {
				 sf::ConvexShape obstacleshape;
				 obstacleshape.setPointCount(8);
				 obstacleshape.setPoint(0, sf::Vector2f(0, 128));
				 obstacleshape.setPoint(1, sf::Vector2f(-20, 20));
				 obstacleshape.setPoint(2, sf::Vector2f(-256, 0));
				 obstacleshape.setPoint(3, sf::Vector2f(-20, -20));
				 obstacleshape.setPoint(4, sf::Vector2f(0, -128));
				 obstacleshape.setPoint(5, sf::Vector2f(20, -20));
				 obstacleshape.setPoint(6, sf::Vector2f(256, 0));
				 obstacleshape.setPoint(7, sf::Vector2f(20, 20));

				 obstacleshape.setPosition(obstacleobje->GetPosition().x * SCALE, obstacleobje->GetPosition().y * SCALE);
				 obstacleshape.setRotation(180 / b2_pi * obstacleobje->GetAngle());

				 obstacleshape.setFillColor(sf::Color(100, 250, 50));
				 Window.draw(obstacleshape);
			 }
			 ////
			 else if (sekilal == '2')
			 {
				 sf::RectangleShape rectangle(sf::Vector2f(420, 20));
				 rectangle.setPosition(obstacleobje->GetPosition().x * SCALE - 200, obstacleobje->GetPosition().y * SCALE - 170);
				 rectangle.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				 rectangle.setFillColor(sf::Color(100, 250, 50));
				 Window.draw(rectangle);
				 sf::RectangleShape rectangle1(sf::Vector2f(420, 20));
				 rectangle1.setPosition(obstacleobje->GetPosition().x * SCALE - 200, obstacleobje->GetPosition().y * SCALE + 150);
				 rectangle1.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				 rectangle1.setFillColor(sf::Color(100, 250, 50));
				 Window.draw(rectangle1);
				 sf::RectangleShape rectangle2(sf::Vector2f(20, 300));
				 rectangle2.setPosition(obstacleobje->GetPosition().x * SCALE + 200, obstacleobje->GetPosition().y * SCALE - 150);
				 rectangle2.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				 rectangle2.setFillColor(sf::Color(100, 250, 50));
				 Window.draw(rectangle2);
			 }
			 /////
			 else if (sekilal == '3')
			 {
				 sf::RectangleShape rectangle(sf::Vector2f(20, 100));
				 rectangle.setPosition(obstacleobje->GetPosition().x * SCALE - 120, obstacleobje->GetPosition().y * SCALE - 200);
				 rectangle.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				 rectangle.setFillColor(sf::Color(100, 250, 50));
				 Window.draw(rectangle);

				 sf::RectangleShape rectangle1(sf::Vector2f(20, 100));
				 rectangle1.setPosition(obstacleobje->GetPosition().x * SCALE + 100, obstacleobje->GetPosition().y * SCALE + 100);
				 rectangle1.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				 rectangle1.setFillColor(sf::Color(100, 250, 50));
				 Window.draw(rectangle1);

				 sf::ConvexShape obstacleshape;
				 obstacleshape.setPointCount(4);
				 obstacleshape.setPoint(0, sf::Vector2f(-100, -100));
				 obstacleshape.setPoint(1, sf::Vector2f(120, 100));
				 obstacleshape.setPoint(2, sf::Vector2f(100, 100));
				 obstacleshape.setPoint(3, sf::Vector2f(-120, -100));


				 obstacleshape.setPosition(obstacleobje->GetPosition().x * SCALE, obstacleobje->GetPosition().y * SCALE);
				 obstacleshape.setRotation(180 / b2_pi * obstacleobje->GetAngle());

				 obstacleshape.setFillColor(sf::Color(100, 250, 50));
				 Window.draw(obstacleshape);
			 }


			 //moving


			 int x_degeri = robot->GetPosition().x*SCALE;
			 int y_degeri = robot->GetPosition().y*SCALE;
			 if (ilki) {
				 //robot->SetLinearVelocity(b2Vec2((motiongraph[yol[say + 1]].coordinate.x - motiongraph[yol[say]].coordinate.x) / (SCALE), (motiongraph[yol[say + 1]].coordinate.y - motiongraph[yol[say]].coordinate.y) / (SCALE)));
				 //printf("%0.f %0.f\n", abs(motiongraph[yol[say + 1]].coordinate.x - motiongraph[yol[say]].coordinate.x),abs( motiongraph[yol[say + 1]].coordinate.y - motiongraph[yol[say]].coordinate.y));
				 float degerx = motiongraph[yol[say + 1]].coordinate.x - motiongraph[yol[say]].coordinate.x;
				 float degery = motiongraph[yol[say + 1]].coordinate.y - motiongraph[yol[say]].coordinate.y;
				 if (abs(degerx) > 100 || abs(degery) > 100)
				 {
					 while (1) {
						 degerx = degerx / 1.5;
						 degery = degery / 1.5;
						 if (abs(degerx) < 100 && abs(degery) < 100)
							 break;
					 }
				 }

				 robot->SetLinearVelocity(b2Vec2(degerx / SCALE, degery / SCALE));
				 say++;

				 x_karsilastir = x_degeri - motiongraph[yol[say]].coordinate.x;
				 y_karsilastir = y_degeri - motiongraph[yol[say]].coordinate.y;
				 ilki = 0;
			 }
			 if (say < yol.size() - 1) {
				 if (x_karsilastir * (x_degeri - motiongraph[yol[say]].coordinate.x) < 0 || y_karsilastir * (y_degeri - motiongraph[yol[say]].coordinate.y) < 0) {


					 printf("%d\n", say);
					 //sf::sleep(sf::seconds(1));
					 if (motiongraph[yol[say + 1]].coordinate.x == motiongraph[yol[say]].coordinate.x && motiongraph[yol[say + 1]].coordinate.y == motiongraph[yol[say]].coordinate.y)
						 say++;
					 ilki = 1;

				 }
			 }


			 Window.display();
		 }

		 printf("simulation ended\n ");
	 
	 printf("graph press 1:");
	////
	char olsunmu;
	cin >> olsunmu;
	if(olsunmu)
	{ 
		sf::RenderWindow Window1(sf::VideoMode(800, 600, 32), "Graph");
		Window1.setFramerateLimit(60);

		

		while (Window1.isOpen()) {

			sf::Event event1;
			while (Window1.pollEvent(event1))
			{

				if (event1.type == sf::Event::Closed)
					Window1.close();
			}
			Window1.clear(sf::Color::White);

			for (int i = 0; i < motiongraph.size(); i++) {
				sf::CircleShape nokta(2);
				nokta.setOrigin(2, 2);
				nokta.setPosition(motiongraph[i].coordinate.x, motiongraph[i].coordinate.y);
				nokta.setFillColor(sf::Color::Blue);
				
				Window1.draw(nokta);

				for (int j = 0; j < motiongraph[i].next.size(); j++) {
					sf::VertexArray lines(sf::LinesStrip, 2);
					lines[0].position = sf::Vector2f(motiongraph[i].coordinate.x, motiongraph[i].coordinate.y);
					lines[1].position = sf::Vector2f(motiongraph[i].next[j]->coordinate.x,motiongraph[i].next[j]->coordinate.y );
					lines[0].color = sf::Color::Black;
					lines[1].color = sf::Color::Black;
					Window1.draw(lines);
				}
			}

			//////////
			if (sekilal == '1')
			{
				sf::ConvexShape obstacleshape;
				obstacleshape.setPointCount(8);
				obstacleshape.setPoint(0, sf::Vector2f(0, 128));
				obstacleshape.setPoint(1, sf::Vector2f(-20, 20));
				obstacleshape.setPoint(2, sf::Vector2f(-256, 0));
				obstacleshape.setPoint(3, sf::Vector2f(-20, -20));
				obstacleshape.setPoint(4, sf::Vector2f(0, -128));
				obstacleshape.setPoint(5, sf::Vector2f(20, -20));
				obstacleshape.setPoint(6, sf::Vector2f(256, 0));
				obstacleshape.setPoint(7, sf::Vector2f(20, 20));

				obstacleshape.setPosition(obstacleobje->GetPosition().x * SCALE, obstacleobje->GetPosition().y * SCALE);
				obstacleshape.setRotation(180 / b2_pi * obstacleobje->GetAngle());

				obstacleshape.setFillColor(sf::Color(100, 250, 50));
				Window1.draw(obstacleshape);
			}
			////
			else if (sekilal == '2')
			{
				sf::RectangleShape rectangle(sf::Vector2f(420, 20));
				rectangle.setPosition(obstacleobje->GetPosition().x * SCALE - 200, obstacleobje->GetPosition().y * SCALE - 170);
				rectangle.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				rectangle.setFillColor(sf::Color(100, 250, 50));
				Window1.draw(rectangle);
				sf::RectangleShape rectangle1(sf::Vector2f(420, 20));
				rectangle1.setPosition(obstacleobje->GetPosition().x * SCALE - 200, obstacleobje->GetPosition().y * SCALE + 150);
				rectangle1.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				rectangle1.setFillColor(sf::Color(100, 250, 50));
				Window1.draw(rectangle1);
				sf::RectangleShape rectangle2(sf::Vector2f(20, 300));
				rectangle2.setPosition(obstacleobje->GetPosition().x * SCALE + 200, obstacleobje->GetPosition().y * SCALE - 150);
				rectangle2.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				rectangle2.setFillColor(sf::Color(100, 250, 50));
				Window1.draw(rectangle2);
			}
			/////
			else if (sekilal == '3')
			{
				sf::RectangleShape rectangle(sf::Vector2f(20, 100));
				rectangle.setPosition(obstacleobje->GetPosition().x * SCALE - 120, obstacleobje->GetPosition().y * SCALE - 200);
				rectangle.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				rectangle.setFillColor(sf::Color(100, 250, 50));
				Window1.draw(rectangle);

				sf::RectangleShape rectangle1(sf::Vector2f(20, 100));
				rectangle1.setPosition(obstacleobje->GetPosition().x * SCALE + 100, obstacleobje->GetPosition().y * SCALE + 100);
				rectangle1.setRotation(180 / b2_pi * obstacleobje->GetAngle());
				rectangle1.setFillColor(sf::Color(100, 250, 50));
				Window1.draw(rectangle1);

				sf::ConvexShape obstacleshape;
				obstacleshape.setPointCount(4);
				obstacleshape.setPoint(0, sf::Vector2f(-100, -100));
				obstacleshape.setPoint(1, sf::Vector2f(120, 100));
				obstacleshape.setPoint(2, sf::Vector2f(100, 100));
				obstacleshape.setPoint(3, sf::Vector2f(-120, -100));


				obstacleshape.setPosition(obstacleobje->GetPosition().x * SCALE, obstacleobje->GetPosition().y * SCALE);
				obstacleshape.setRotation(180 / b2_pi * obstacleobje->GetAngle());

				obstacleshape.setFillColor(sf::Color(100, 250, 50));
				Window1.draw(obstacleshape);
			}

			sf::CircleShape robot_shape(25);
			robot_shape.setOrigin(25, 25);
			robot_shape.setPosition(motiongraph[0].coordinate.x, motiongraph[0].coordinate.y);
			robot_shape.setFillColor(sf::Color::Blue);
			//robot_shape.setRotation(robot->GetAngle() * 180 / b2_pi);
			Window1.draw(robot_shape);

			sf::RectangleShape object1_shape(sf::Vector2f(50, 50));
			object1_shape.setOrigin(25, 25);
			object1_shape.setPosition(motiongraph[motiongraph.size()-1].coordinate.x, motiongraph[motiongraph.size() - 1].coordinate.y);
			object1_shape.setFillColor(sf::Color::Red);
			//object1_shape.setRotation(object1->GetAngle() * 180 / b2_pi);
			Window1.draw(object1_shape);

			for (int i = 0; i<yol.size()-1; i++) {
				sf::VertexArray lines1(sf::LinesStrip, 2);
				lines1[0].position = sf::Vector2f(motiongraph[yol[i]].coordinate.x, motiongraph[yol[i]].coordinate.y);
				lines1[1].position = sf::Vector2f(motiongraph[yol[i+1]].coordinate.x, motiongraph[yol[i+1]].coordinate.y);
				lines1[0].color = sf::Color::Yellow;
				lines1[1].color = sf::Color::Yellow;
				
				Window1.draw(lines1);
				
			}

			Window1.display();
		}
	}
	////
	vector<node>().swap(motiongraph);
	system("PAUSE");
	return 0;
}
