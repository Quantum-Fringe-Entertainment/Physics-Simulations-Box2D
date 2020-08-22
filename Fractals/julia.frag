#version 410 core

out vec4 color;

uniform double screen_ratio;
uniform dvec2 screen_size;
uniform dvec2 center;
uniform double zoom;
uniform int itr;

vec4 map_to_color(float t) {
    float r = 64 * (1.0 - t) * t * t * t;
    float g = 32 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 5 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

    return vec4(r, g, b, 1.0);
}

void main()
{
    dvec2 z, c;
    c.x = (gl_FragCoord.x / screen_size.x - 0.5);
    c.y = (gl_FragCoord.y / screen_size.y - 0.5);

    c.x /= zoom;
    c.y /= zoom;

    c.x += center.x;
    c.y += center.y;

    int i;
    for(i = 0; i < itr; ++i) {
        double x = ((z.x * z.x * z.x) - (3 * z.x * z.y *z.y)) + c.x ;
		double y = ((3 * z.x * z.x * z.y) - ( z.y * z.y * z.y)) + c.y;

		if((x * x + y * y) > 4.0) break;
		z.x = x;
		z.y = y;
    }

    double t = double(i) / double(itr);

    color = map_to_color(float(t));
}
