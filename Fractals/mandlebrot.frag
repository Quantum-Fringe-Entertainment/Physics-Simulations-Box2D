#version 410 core

out vec4 color;

uniform double screen_ratio;
uniform dvec2 screen_size;
uniform dvec2 center;
uniform double zoom;
uniform int itr;

vec4 map_to_color(float t) {
    float r = 4 * (1.0 - t) * t * t * t;
    float g = 4 * (1.0 - t) * (1.0 - t) * t * t;
    float b = 1 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t;

    return vec4(r, g, b, 1.0);
}

void main()
{
    dvec2 z, c;
    c.x = screen_ratio * (gl_FragCoord.x / screen_size.x - 0.5);
    c.y = (gl_FragCoord.y / screen_size.y - 0.5);

    c.x /= zoom;
    c.y /= zoom;

    c.x += center.x;
    c.y += center.y;

    int i;
    for(i = 0; i < itr; i++) {
        double x = (z.x * z.x - z.y * z.y) + c.x;
		double y = (z.y * z.x + z.x * z.y) + c.y;

		if((x * x + y * y) > 4.0) break;
		z.x = x;
		z.y = y;
    }

    double t = double(i) / double(itr);

    color = map_to_color(float(t));
    // color = vec4(0.5, 1, 1, 1);
}
