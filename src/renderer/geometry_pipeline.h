#pragma once

#include "render_pipelines.h"

class GeometryPipeline : public RenderPipeline {
public:
    void Prepare(Camera* camera = nullptr) override;
    void Update(Camera* camera) override;
};
