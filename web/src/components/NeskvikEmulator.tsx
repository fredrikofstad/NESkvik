import { useEffect, useState } from 'react'
import { Canvas, useFrame } from '@react-three/fiber'
import { OrbitControls } from '@react-three/drei'
import TV from './TV'
import createEmuModule from '../wasm/neskvik.js'

type EmulatorState = {
    Module: any
    runFrame: () => void
    framebuffer: Uint8Array
    width: number
    height: number
}

interface NeskvikModule {
    cwrap: any
    getValue: any
    HEAPU8: Uint8Array
}

export default function NeskvikEmulator() {
    const [emu, setEmu] = useState<EmulatorState | null>(null)

    useEffect(() => {
        createEmuModule().then((Module: NeskvikModule) => {
            const initEmu = Module.cwrap('initEmu', 'number', [])
            const runFrame = Module.cwrap('runFrame', null, [])
            const getFramebufferPtr = Module.cwrap('getFramebufferPtr', 'number', [])

            const resPtr = initEmu()
            runFrame();
            const width = Module.getValue(resPtr, 'i32')
            const height = Module.getValue(resPtr + 4, 'i32')
            const fbPtr = getFramebufferPtr()
            const framebuffer = Module.HEAPU8.subarray(fbPtr, fbPtr + width * height * 4)

            for (let i = 0; i < framebuffer.length; i += 4) {
                framebuffer[i] = 255
                framebuffer[i+1] = 0
                framebuffer[i+2] = 0
                framebuffer[i+3] = 255
            }

            setEmu({
                Module,
                runFrame,
                framebuffer,
                width,
                height,
            })
        })
    }, [])

    return (
        <Canvas camera={{ position: [0, 0, 3.5] }}>
            <ambientLight intensity={3} />
            <directionalLight intensity={2} position={[1, 1, 1]} />
            {emu && (
                <>
                    <TV
                        framebuffer={emu.framebuffer}
                        width={emu.width}
                        height={emu.height}
                    />
                    <EmuUpdater emu={emu} />
                </>
            )}
            <OrbitControls />
        </Canvas>
    )
}

function EmuUpdater({ emu }: { emu: EmulatorState }) {
    useFrame(() => {
        emu.runFrame()
    })
    return null
}
