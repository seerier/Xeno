#pragma once

#include "shape.h"
#include "triangle.h"

namespace xeno {

struct VertRef {
    VertRef(int v, int vt, int vn) :v(v), vt(vt), vn(vn) {}
    int v, vt, vn;
};

struct ObjTriangleMesh {
    ObjTriangleMesh() {}
    bool createFromObjFile(const std::string &path) {
        

        // If the file is not an .obj file return false
	    if (path.substr(path.size() - 4, 4) != ".obj")
            return false;

        std::ifstream file(path);
        if (!file.is_open())
            return false;

        p.clear();
        n.clear();
        uv.clear();
        indices.clear();
        nFaces = 0;
        nVertices = 0;

        std::string curline;
        while (std::getline(file, curline)) {
            std::istringstream lineSS(curline);
            std::string lineType;
            lineSS >> lineType;

            // vertex
            if (lineType == "v") {
                float x, y, z;
                lineSS >> x >> y >> z;
                p.push_back(Point3f(x, y, z));
                ++nVertices;
            }

            // texture
            if (lineType == "vt") {
                float u = 0, v = 0;
                lineSS >> u >> v;
                uv.push_back(Point2f(u, v));
            }

            // normal
            if (lineType == "vn") {
                float i = 0, j = 0, k = 0;
                lineSS >> i >> j >> k;
                n.push_back(Normal3f(i, j, k));
            }

            // face
            if (lineType == "f") {
                std::string refStr;
                std::vector<VertRef> localVertRef;

                while (lineSS >> refStr) {
                    std::stringstream ref(refStr);
                    std::string vStr, vtStr, vnStr;
                    std::getline(ref, vStr, '/');
                    std::getline(ref, vtStr, '/');
                    std::getline(ref, vnStr, '/');
                    int vIndex = std::stoi(vStr);
                    int vtIndex = vtStr.empty() ? 0 : std::stoi(vtStr);
                    int vnIndex = vnStr.empty() ? 0 : std::stoi(vnStr);
                    vIndex = (vIndex > 0 ? vIndex : vIndex + p.size()) - 1; 
                    vtIndex = (vIndex > 0 ? vtIndex : vtIndex + p.size()) - 1; // should handle the special case that vtIndex==0, leave for future development
                    vnIndex = (vIndex > 0 ? vnIndex : vnIndex + p.size()) - 1;
                    localVertRef.emplace_back(vIndex, vtIndex, vnIndex);
                }

                for (int i = 1;i + 1 < localVertRef.size();++i) {
                    indices.push_back(localVertRef[0]);
                    indices.push_back(localVertRef[i]);
                    indices.push_back(localVertRef[i + 1]);
                    ++nFaces;
                }

                /*
                for (int num = 0; num < 3; ++num) {
                    lineSS >> refStr;
                    std::stringstream ref(refStr);
                    std::string vStr, vtStr, vnStr;
                    std::getline(ref, vStr, '/');
                    std::getline(ref, vtStr, '/');
                    std::getline(ref, vnStr, '/');
                    int vIndex = std::stoi(vStr);
                    int vtIndex = vtStr.empty() ? 0 : std::stoi(vtStr);
                    int vnIndex = vnStr.empty() ? 0 : std::stoi(vnStr);
                    vIndex = (vIndex > 0 ? vIndex : vIndex + p.size()) - 1;
                    vtIndex = (vIndex > 0 ? vtIndex : vtIndex + p.size()) - 1;
                    vnIndex = (vIndex > 0 ? vnIndex : vnIndex + p.size()) - 1;
                    indices.emplace_back(vIndex, vtIndex, vnIndex);
                }
                */
            }
        }
        return true;
    }

    std::vector<Triangle> toTriangleList() {
        std::vector<Triangle> triangleList;
        for (int i = 0; i < nFaces; ++i) {
            triangleList.emplace_back(p[indices[3 * i].v], p[indices[3 * i + 1].v], p[indices[3 * i + 2].v]);
        }
        return triangleList;
    }

    std::vector<std::shared_ptr<Triangle>> toSharedTriangleList() {
        std::vector<std::shared_ptr<Triangle>> sharedTriangleList;
        for (int i = 0; i < nFaces; ++i) {
            sharedTriangleList.emplace_back(std::make_shared<Triangle>(p[indices[3 * i].v], p[indices[3 * i + 1].v], p[indices[3 * i + 2].v]));
        }
        return sharedTriangleList;
    }

    int nFaces = 0;
    int nVertices = 0;
    std::vector<Point3f> p;
    std::vector<Normal3f> n;
    std::vector<Point2f> uv;
    std::vector<VertRef> indices;
};



}