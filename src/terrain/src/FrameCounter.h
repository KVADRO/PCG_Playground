#pragma once

#include <chrono>
#include <thread>

namespace vv {
class FrameCounter
{
public:
  struct InitInfo
  {
    float targetFrameRate{ 0.0f };
  };

public:
  void init(const InitInfo& _info)
  {
    m_TargetFrameRate = _info.targetFrameRate;
  }

  void beginFrame()
  {
    m_BeginFrameTimestamp = std::chrono::high_resolution_clock::now();
  }

  void endFrame()
  {
    const int64_t frameDeltaMcS =
      std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - m_BeginFrameTimestamp)
        .count();

    const int64_t targetFrameDeltaMcS =
      static_cast<int64_t>(m_TargetFrameRate * 1000000);

    const int64_t currentTargetFrameDeltaDiffMcS =
      targetFrameDeltaMcS - frameDeltaMcS;

    if (currentTargetFrameDeltaDiffMcS > 0) {
      std::this_thread::sleep_for(
        std::chrono::microseconds{ currentTargetFrameDeltaDiffMcS });

      m_LastFrameDelta =
        static_cast<float>(frameDeltaMcS + currentTargetFrameDeltaDiffMcS) /
        1000000.0f;
    } else {
      m_LastFrameDelta = static_cast<float>(frameDeltaMcS) / 1000000.0f;
    }
  }

  float getLastFrameDelta() const { return m_LastFrameDelta; }

private:
  std::chrono::high_resolution_clock::time_point m_BeginFrameTimestamp;

  float m_LastFrameDelta{ 0.0f };
  float m_TargetFrameRate{ 0.0f };
};
}