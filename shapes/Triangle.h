#ifndef TRIANGLE_H
#define TRIANGLE_H

#include <glm/glm.hpp>
#include <initializer_list>
typedef glm::vec3 Normal;

class Triangle
{
public:
    Triangle(glm::vec3 a, glm::vec3 b, glm::vec3 c);
    void transform(glm::mat4 transformation);
    inline glm::vec3 a() { return m_a; }
    inline glm::vec3 b() { return m_b; }
    inline glm::vec3 c() { return m_c; }
    inline void overrideANormal(Normal normal) { m_isAOverride = true; m_aNormal = normal; }
    inline void overrideBNormal(Normal normal) { m_isBOverride = true; m_bNormal = normal; }
    inline void overrideCNormal(Normal normal) { m_isCOverride = true; m_cNormal = normal; }
    inline Normal defaultNormal() { return m_defaultNormal; }
    inline Normal aNormal() { return m_isAOverride ? m_aNormal : m_defaultNormal; }
    inline Normal bNormal() { return m_isBOverride ? m_bNormal : m_defaultNormal; }
    inline Normal cNormal() { return m_isCOverride ? m_cNormal : m_defaultNormal; }
    static Normal averageNormals(const std::initializer_list<Normal>& normals);
private:
    glm::vec3 m_a;
    glm::vec3 m_b;
    glm::vec3 m_c;
    bool m_isAOverride = false;
    bool m_isBOverride = false;
    bool m_isCOverride = false;
    Normal m_aNormal;
    Normal m_bNormal;
    Normal m_cNormal;
    Normal m_defaultNormal;
    void computeDefaultNormal();
};

#endif // TRIANGLE_H
