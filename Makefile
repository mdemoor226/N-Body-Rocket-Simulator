all: Space Shoot

Space:
	g++ -pthread --std=c++0x -Wall Celestial_Body.cpp Celestial.cpp Celestial_Vec.cpp Orbit_Generator.cpp Orbit_Generator_Settings.cpp Verify_Input.cpp -o Space
Shoot:
	g++ --std=c++0x Shoot.cpp -o Shoot
