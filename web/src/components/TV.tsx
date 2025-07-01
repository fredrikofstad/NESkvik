    import { useGLTF } from '@react-three/drei'
    import { useFrame } from '@react-three/fiber'
    import { useEffect, useRef } from 'react'
    import * as THREE from 'three'

    type TVModelProps = {
        framebuffer: Uint8Array
        width: number
        height: number
    }

    export default function TV({ framebuffer, width, height }: TVModelProps) {
        const gltf = useGLTF('/tv.glb')

        const textureRef = useRef(
            new THREE.DataTexture(framebuffer, width, height, THREE.RGBAFormat)
        )

        useEffect(() => {
            textureRef.current.magFilter = THREE.NearestFilter
            textureRef.current.minFilter = THREE.NearestFilter
            textureRef.current.flipY = true
            textureRef.current.colorSpace = THREE.NoColorSpace // crucial for raw pixel data
            textureRef.current.needsUpdate = true

            gltf.scene.traverse((obj) => {
                if (
                    obj instanceof THREE.Mesh &&
                    obj.material &&
                    !Array.isArray(obj.material) &&
                    obj.material.name === 'screen'
                ) {
                    obj.material.map = textureRef.current
                    obj.material.needsUpdate = true
                }
            })
        }, [gltf])

        useFrame(() => {
            textureRef.current.needsUpdate = true
        })

        return <primitive object={gltf.scene} />
    }
