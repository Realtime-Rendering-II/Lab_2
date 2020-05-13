#version 330 core

in vec3 position;
in vec3 normal;
in vec2 tex_coord;

uniform sampler2D custom_texture_1;

//LD
uniform vec3 light_diffuse_color;

uniform vec3 light_position;
uniform vec3 camera_position;

out vec4 out_color;

void main(){
    //KD
    vec3 diffuse_object_color = texture(custom_texture_1, tex_coord).rgb;

    //for the specular and ambient object color you can assume that it is identical with the diffuse object color

    vec3 normal = normalize(normal);
    vec3 light_dir = normalize(light_position - position);

    float attenuation_factor = 1.0f / (1 + pow(length(light_dir), 2));
    vec3 diffuse = attenuation_factor * light_diffuse_color * diffuse_object_color * max(dot(light_dir, normal), 0.0);

    //TODO 2: extend this shader to compute Blinn-Phong shading

    //TODO 5: implement Phong shading
    //TODO 6: modify this shader to switch between Phong and Blinn-Phong shading at runtime using the boolean that was uploaded

    //TODO 10: modify this shader to take two light sources into account (do not forget the attenuation factor)

    //TODO 3: clamp the color values to the interval [0,1] before returning the fragment color
    out_color = vec4(diffuse, 1.0);
}