#include "common.h"
#include <vector>
#include "Vertex.h"

class Line{


private:
	std::vector<Vertex>	d_vertices;
	GLuint			d_VAO;
	GLuint			d_VBO; 

	void Init();
	 
public:

	Line(Vertex origin, Vertex target);

	~Line();

	void Draw();

	
};