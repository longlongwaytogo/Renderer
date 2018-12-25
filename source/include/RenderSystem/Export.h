#ifndef Render_INCLUDE_RENDERSYSTEM_EXPORT_H
#define Render_INCLUDE_RENDERSYSTEM_EXPORT_H

#ifdef RENDERSYSTEM_EXPORTS
#define RENDERSYSTEM_API __declspec(dllexport)
#else
#define RENDERSYSTEM_API __declspec(dllimport)
#endif

#endif // Render_INCLUDE_RENDERSYSTEM_EXPORT_H
