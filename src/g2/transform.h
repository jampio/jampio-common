#pragma once

#include "mdx.h"

// fwd declare
class CBoneCache;

void G2_TransformBone(int child, CBoneCache &BC);
void Multiply_3x4Matrix(mdxaBone_t *out, mdxaBone_t *in2, mdxaBone_t *in);