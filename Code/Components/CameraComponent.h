#pragma once

enum class ProjectionType
{
	Perspective,
	Orthographic
};

struct CameraComponent
{
	ProjectionType projection = ProjectionType::Orthographic;
	float size = 1.0f;
	float near = 0.0f;
	float far = 1000.0f;
};