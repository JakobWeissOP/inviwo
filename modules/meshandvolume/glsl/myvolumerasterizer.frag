/*********************************************************************************
 *
 * Inviwo - Interactive Visualization Workshop
 *
 * Copyright (c) 2019-2021 Inviwo Foundation
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *********************************************************************************/

#include "utils/shading.glsl"

uniform LightParameters lighting;
uniform CameraParameters camera;

in fData {
    vec4 worldPosition;
    vec4 position;
    vec3 normal;
#ifdef SEND_COLOR
    vec4 color;
#endif
#ifdef SEND_TEX_COORD
    vec2 texCoord;
#endif
#ifdef SEND_SCALAR
    float scalar;
#endif
    float area;
#ifdef ALPHA_SHAPE
    vec3 sideLengths;
#endif
#if defined(DRAW_EDGES) || defined(DRAW_SILHOUETTE)
    vec3 edgeCoordinates;
#endif
#ifdef DRAW_SILHOUETTE
    flat bvec3 silhouettes;
#endif
}
frag;

#ifdef USE_FRAGMENT_LIST
#include "oit/abufferlinkedlist.glsl"

// this is important for the occlusion query
layout(early_fragment_tests) in;

layout(pixel_center_integer) in vec4 gl_FragCoord;
#endif



void main() {
    vec4 fragColor = performShading();

#ifdef USE_FRAGMENT_LIST
    // fragment list rendering
    if (fragColor.a > 0.0) {
        ivec2 coords = ivec2(gl_FragCoord.xy);
        float depth = gl_FragCoord.z;
        abufferRender(coords, depth, fragColor);
    }
    discard;

#else
    // traditional rendering
    FragData0 = fragColor;
    PickingData = vec4(0.0, 0.0, 0.0, 0.0);
#endif
}
