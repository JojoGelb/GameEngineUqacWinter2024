#version 450

/* Rendu du jeu */
layout(binding = 0) uniform sampler2D uSampler;

/* Texture de déformation en rouge et vert */
layout(binding = 1) uniform sampler2D uDeformation;

/* Texture pour contrôler l'intensité de la déformation */
layout(binding = 2) uniform sampler2D uIntensity;

layout(binding = 3) uniform uSettings {
    /* Interval de temps multiplié par la vitesse depuis l'activation du composant */
    float uTime;

    /* Échelle de la déformation */
    float uScale;
};

/* Coordonnées UV du fragment */
layout(location = 0) in vec2 vTextureCoord;

/* Couleur de sortie */
layout(location = 0) out vec4 fragColor;

void main(void) {

    //Etape 1: récupère l'intensité de la déformation 
    vec2 deformationIntensity = texture(uIntensity, vec2(uTime, 0.5)).xy;
    deformationIntensity *= uScale;

    //Step 2: récupère le vecteur de déformation
    vec2 deformationVector = texture(uDeformation, vTextureCoord + sin(uTime)).xy;
    //Centre la deformation ([0,1] -> [-0.5,0.5]) et la module par l'intensité
    deformationVector = (deformationVector -0.5) * deformationIntensity;
    
    //Step 3: applique la déformation
    fragColor = texture( uSampler, vTextureCoord + deformationVector);
}
