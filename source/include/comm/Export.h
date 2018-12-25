#ifndef Render_INCLUDE_COMM_EXPORT_H
#define Render_INCLUDE_COMM_EXPORT_H

#ifdef COMM_EXPORTS
#define COMM_API __declspec(dllexport)
#else
#define COMM_API __declspec(dllimport)
#endif

#endif // Render_INCLUDE_COMM_EXPORT_H
