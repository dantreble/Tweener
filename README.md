# Tweener
Tweener is a Unreal Engine plugin for animating properties. It works independently of using a tick or a animation. It doesn't require any initialisation.

Each tween is available as a async task, or can be created from the tweener subsystem.

It can tween
* Actor/SceneComponent
  * Location
  * Rotation
  * Scale

* Material
  * Color/Vector
  * Scalar

* Widget
  * RenderLocation
  * RenderScale
  * RenderShear
  * RenderAngle
  * RenderOpacity
  * Color
 
* Property
  * Float
  * Vector


It started as a port of [GoKitLite](https://github.com/prime31/GoKitLite) for Unreal 