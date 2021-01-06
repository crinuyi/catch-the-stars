float sign (glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    return (p1.x - p3.x) * (p2.z - p3.z) - (p2.x - p3.x) * (p1.z - p3.z);
}

class Triangle {
    public:
        glm::vec3 pointA;
        glm::vec3 pointB;
        glm::vec3 pointC;
        float a, b, c, d; //równanie płaszczyzny, na której leży trójkąt
        Triangle(glm::vec3 pointA, glm::vec3 pointB, glm::vec3 pointC);
        float altitude(glm::vec3 point);
        int isInside(glm::vec3 point);
};

Triangle::Triangle(glm::vec3 pointA,
                   glm::vec3 pointB,
                   glm::vec3 pointC) {
    this->pointA = pointA;
    this->pointB = pointB;
    this->pointC = pointC;

    glm::vec3 vectorA, vectorB;

    vectorA = pointB - pointA;
    vectorB = pointC - pointA;

    this->a = vectorA.z * vectorB.y - vectorA.y * vectorB.z; // A
    this->b = vectorA.y * vectorB.x - vectorA.x * vectorB.y; // B
    this->c = vectorA.x * vectorB.z - vectorA.z * vectorB.x; // C
    this->d = -(a * pointA.x + b * pointA.z + c * pointA.y); // D
}

float Triangle::altitude(glm::vec3 point) {
    if (c) 
        return -(a*point.x + b*point.z + d)/c;
    else 
        return -INFINITY;
}

int Triangle::isInside(glm::vec3 point) {
    float s1, s2, s3;
    bool hasNeg, hasPos;

    s1 = sign(point, pointA, pointB);
    s2 = sign(point, pointB, pointC);
    s3 = sign(point, pointC, pointA);

    hasNeg = (s1 < 0) || (s2 < 0) || (s3 < 0);
    hasPos = (s1 > 0) || (s2 > 0) || (s3 > 0);

    return !(hasNeg && hasPos) && c; //jeśli trójkąt jest pionowy
}

class Ground {
    public:
        std::vector<Triangle> triangles;
        float getAltitude(glm::vec3 point); 

        Ground(const char* path);
};

Ground::Ground(const char* path) {
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> uvs;
    std::vector<glm::vec3> normals;

    loadOBJ(path, vertices, uvs, normals);

    for(int i=0; i<vertices.size(); i+=3) {
        Triangle triangle(
            vertices[i],
            vertices[i+1],
            vertices[i+2]);
        this->triangles.push_back(triangle);
    }
}

float Ground::getAltitude(glm::vec3 point) {
    std::vector<Triangle> intersectedTriangles;
    for(auto t : this->triangles)
        if(t.isInside(point))
            intersectedTriangles.push_back(t);

    float pointGroundAltitude = point.y;

    float closestAltitude = INFINITY;
    float smallestAltitudeDifference = INFINITY;

    if(intersectedTriangles.size()==0)
        return -INFINITY;


    for(auto t : intersectedTriangles) {
        float tAltitude = t.altitude(point);
        if(tAltitude > point.y && tAltitude < (point.y+1.0f)) //do preferowania trójkąta, który jest pomiędzy "okiem a butem"
            return tAltitude;

        if(abs(pointGroundAltitude-tAltitude)<smallestAltitudeDifference) {
            closestAltitude = tAltitude;
            smallestAltitudeDifference = abs(pointGroundAltitude-tAltitude);
        }
    }

    return closestAltitude;
}