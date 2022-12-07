#pragma once

#include "render_pipelines.h"

class GeometryPipeline : public RenderPipeline {
public:
    void Prepare() override;
    void Update() override;
};
