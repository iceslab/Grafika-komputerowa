#include "Sphere.hpp"

istream& operator>>(istream& in, Sphere& s)
{
	in >> s.radius >> flow(s.position, >>);
	in >> flow(s.specular, >>);
	in >> flow(s.diffuse, >>);
	in >> flow(s.ambient, >>);
	in >> s.shine;
	return in;
}

ostream& operator<<(ostream& out, Sphere& s)
{
	out << "Sphere: \n\tPosition: ( "<< flow(s.position, <<", "<<) <<" )\n";
	out << "\tRadius: " << s.radius<<"\n";
	return out;
}

void Sphere::rotateZ(double angle)
{
	double x = position[0] * cos(angle) - position[1] * sin(angle);
	double y = position[0] * sin(angle) + position[1] * cos(angle);
	position[0] = x;
	position[1] = y;
}

void Sphere::rotateX(double angle)
{
	position[2] += 5;
	double z = position[0] * cos(angle) - position[2] * sin(angle);
	double y = position[0] * sin(angle) + position[2] * cos(angle);
	position[0] = z;
	position[2] = y;
	position[2] -= 5;
}