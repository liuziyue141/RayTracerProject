#include "raytracer.h"
using namespace std;
struct Ray {
    vec3 P0;
    vec3 P1;
};

//using namespace glm;
int Intersect(Ray &ray){
    int objectId = -1;
    vec3 P_return;
    float min_t = FLT_MAX;
    for (int i = 0 ; i < numobjects ; i++) {
        object* obj = &(objects[i]);
        if(obj->type == "sphere"){
            float a = glm::dot(ray.P1, ray.P1);
            float b = 2.0f * glm::dot(ray.P1, ray.P0-(obj->sphere_loc));
            float c = glm::dot(ray.P0 - obj->sphere_loc, ray.P0 - obj->sphere_loc) - obj->sphere_rad*obj->sphere_rad;
            float discriminant = b*b-4*a*c;
            if(discriminant>=0){
                float root_minus = (-b-sqrt(discriminant))/(2*a);
                float root_plus = (-b+sqrt(discriminant))/(2*a);
                if(root_minus>0&&min_t>root_minus){
                    min_t = root_minus;
                    objectId = i;
                }else if (root_plus>0&&min_t>root_plus){
                    min_t = root_plus;
                    objectId = i;
                }
            }
        }else{
            vec3 A = obj->tri_v1;
            vec3 B = obj->tri_v2;
            vec3 C = obj->tri_v3;
            //vec3 normal = glm::cross((B-A), (C-A));//need to verify
            vec3 n = glm::normalize(glm::cross((C-A), (B-A)));
            float t = (glm::dot(A, n)-glm::dot(ray.P0, n))/glm::dot(ray.P1, n);
            if(t<min_t){
                vec3 P = ray.P0 + ray.P1*t ;
                float det = (C.x - A.x)*(B.y - A.y) - (B.x - A.x)*(C.y - A.y);
                float factor_beta = (C.x - A.x)*(P.y - A.y) - (P.x - A.x)*(C.y - A.y);
                float factor_gamma = (B.y - A.y)*(P.x - A.x) - (B.x - A.x)*(P.y - A.y);
                float beta = factor_beta / det;
                float gamma = factor_gamma / det;
                float alpha = 1.0 - gamma - beta;
                if(alpha>=0 && beta>=0 && gamma >= 0){
                //                     cout << "beta value: " << beta << endl;
                // cout << "alpha value: " << alpha << endl;
                // cout << "gamma value: " << gamma << endl;
                // cout << endl;
                    min_t = t;
                    objectId = i;
                    P_return = P;
                }      
            }
        }
    }
    ray.P0 = P_return;
    return objectId; 
}
Ray RayThruPixel(int i, int j){
    float alpha = tan(fovx/2.0f)*((i-(w/2.0f))/(w/2.0f));
    float beta = tan(fovy/2.0f)*((h/2.0f)-j)/(h/2.0f);
    vec3 w = glm::normalize(eye - center);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::cross(w, u);
    Ray ray;
    ray.P0 = eye;
    ray.P1 = glm::normalize(alpha*u+beta*v-w);
    return ray;
}
void traceRay(){
    //BYTE pixels[h][w];
    FIBITMAP * bitmap = FreeImage_Allocate(w, h, 24);
    //modelview = glm::lookAt(eye, center, up);
    // for (int i = 0 ; i < numobjects ; i++) {
    //     object* obj = &(objects[i]);
    //     obj->transform = modelview*obj->transform;//we need to modify this for scale
    // }
    // Ray ray;
    RGBQUAD color;
    Ray ray = RayThruPixel(0, w/2.0f);
    cout << ray.P0[0] << " " << ray.P0[1] << " " << ray.P0[2] << endl;
    cout << ray.P1[0] << " " << ray.P1[1] << " " << ray.P1[2] << endl;
    //if this is too slow, we change h and w order
    for(int i = 0; i<w; i++){
        for(int j = 0; j<h; j++){
            Ray ray = RayThruPixel(i, j);
            int objectId = Intersect(ray);
            //cout << objectId << endl;
            if(objectId>=0){
                //cout << "reached this line" << endl;
                object* obj = &(objects[objectId]);
                color.rgbRed = (double) obj->diffuse[2]*255.0;
                //color.rgbRed = 255;
                color.rgbGreen = (double) obj->diffuse[1]*255.0;
                //color.rgbGreen = 0;
                color.rgbBlue = (double) obj->diffuse[0]*255.0;
                //color.rgbBlue = 0;
                //pixels[i][j] =0xFF0000;
            }else{
                color.rgbRed = 0;
                //color.rgbGreen = (double) obj->diffuse[1]*255.0;
                color.rgbGreen = 0;
                //color.rgbBlue = (double) obj->diffuse[2]*255.0;
                color.rgbBlue = 0;
                //pixels[i][j] =0x000000;

            }
            FreeImage_SetPixelColor(bitmap, i, h-j-1, &color);
        
        }
    }
    //FIBITMAP *bitmap = FreeImage_ConvertFromRawBits(pixels[0], w, h, w * 3, 24, 0xFF0000, 0x00FF00, 0x0000FF, true);

    if (FreeImage_Save(FIF_PNG, bitmap, "test.png", 0)){
        cout << "Image successfully saved!" << endl;
    }
}



