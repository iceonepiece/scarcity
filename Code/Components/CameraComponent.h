#pragma once

enum class ProjectionType
{
	Perspective,
	Orthographic
};

struct CameraComponent
{
	ProjectionType projection = ProjectionType::Perspective;
	float near = 0.0f;
	float far = 1000.0f;
};