varying vec2  uv;
// this vertex shader is from AMD RenderMonkey
void main(void)
{
   gl_Position = vec4( gl_Vertex.xy, 0.0, 1.0 );
   gl_Position = sign( gl_Position );
   
   // Texture coordinate for screen aligned (in correct range):
   uv = (vec2( gl_Position.x, - gl_Position.y ) + vec2( 1.0 ) ) / vec2( 2.0 );
}