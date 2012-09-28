#include <three/three.hpp>

#include <three/extras.hpp>

#include <three/extras/anim.hpp>
#include <three/extras/sdl.hpp>
#include <three/extras/glew.hpp>

#ifndef THREE_ASSET_DIR
#define THREE_ASSET_DIR "."
#endif

using namespace three;

void performance_static( GLRenderer::Ptr renderer ) {

  auto camera = PerspectiveCamera::create(
    60, (float)renderer->width() / renderer->height(), 1, 10000
  );
  camera->position.z = 3200;

  auto scene = Scene::create();

  auto material = MeshNormalMaterial::create(
    Material::Parameters().add("shading", THREE::SmoothShading)
  );

  auto loader = JSONLoader::create();
  loader->load( std::string(THREE_ASSET_DIR) + "/obj/Suzanne.js", [&material]( Geometry::Ptr geometry ) {
    geometry->computeVertexNormals();

    for ( int = 0; i < 7700; i ++ ) {

      auto mesh = Mesh::create( geometry, material );

      mesh->position.x = Math.random() * 10000 - 5000;
      mesh->position.y = Math.random() * 10000 - 5000;
      mesh->position.z = Math.random() * 10000 - 5000;
      mesh->rotation.x = Math.random() * 360 * ( Math.PI / 180 );
      mesh->rotation.y = Math.random() * 360 * ( Math.PI / 180 );
      mesh->scale.x = mesh->scale.y = mesh->scale.z = Math.random(100.f, 150.f);
      mesh->matrixAutoUpdate = false;
      mesh->updateMatrix();

      scene.add( mesh );

    }
  });

  auto mesh = ParticleSystem::create( geometry, material );
  scene->add( mesh );


  auto mouseX = 0.f, mouseY = 0.f;

  anim::gameLoop (

    [&]( float ) -> bool {
      SDL_Event event;
      while ( SDL_PollEvent( &event ) ) {
        switch( event.type ) {
          case SDL_KEYDOWN:
          case SDL_QUIT:
            return false;
          case SDL_MOUSEMOTION:
            mouseX = (float)event.motion.x;
            mouseY = (float)event.motion.y;
          default:
          break;
        };
      }

      camera->position.x += (  mouseX - camera->position.x ) * .1f;
      camera->position.y += ( -mouseY - camera->position.y ) * .1f;
      camera->lookAt( scene->position );

      renderer->render( *scene, *camera );
      sdl::swapBuffers();
      return true;

  } );

}

int main ( int argc, char* argv[] ) {

  auto onQuit = defer( SDL_Quit );

  GLRenderer::Parameters parameters;

  if ( !sdl::initSDL( parameters ) || !glew::initGLEW( parameters ) ) {
    return 0;
  }

  auto renderer = three::GLRenderer::create( parameters );
  if ( !renderer ) {
    return 0;
  }

  renderer->sortObjects = false;

  performance_static( renderer );

  return 0;
}