// Copyright (c) FIRST and other WPILib contributors.
// Open Source Software; you can modify and/or share it under the terms of
// the WPILib BSD license file in the root directory of this project.

#include "frc/GenericHID.h"

#include <string>

#include <hal/DriverStation.h>

#include "frc/DriverStation.h"
#include "frc/Errors.h"
#include "frc/event/BooleanEvent.h"

using namespace frc;

GenericHID::GenericHID(int port) {
  if (port < 0 || port >= DriverStation::kJoystickPorts) {
    throw FRC_MakeError(warn::BadJoystickIndex, "port {} out of range", port);
  }
  m_port = port;
}

bool GenericHID::GetRawButton(int button) const {
  m_disconnectedAlert.Set(!IsConnected());

  if(button > GetButtonCount()) {
    if(m_buttonAlerts.find(button) == m_buttonAlerts.end()) {
      m_buttonAlerts[button] = Alert {
        "JoystickAlerts/" + std::to_string(m_port),
        "Button " + std::to_string(button) + " does not exist on Joystick " + std::to_string(m_port),
        Alert::AlertType::kWarning
      };

    }
    m_buttonAlerts[button].Set(true);
  } else {
    if(m_buttonAlerts.find(button) != m_buttonAlerts.end()) {
      m_buttonAlerts[button].Set(false);
    }
  }

  return DriverStation::GetStickButton(m_port, button);
}

bool GenericHID::GetRawButtonPressed(int button) {
  m_disconnectedAlert.Set(!IsConnected());

  if(button > GetButtonCount()) {
    if(m_buttonAlerts.find(button) == m_buttonAlerts.end()) {
      m_buttonAlerts[button] = Alert {
        "JoystickAlerts/" + std::to_string(m_port),
        "Button " + std::to_string(button) + " does not exist on Joystick " + std::to_string(m_port),
        Alert::AlertType::kWarning
      };

    }
    m_buttonAlerts[button].Set(true);
  } else {
    if(m_buttonAlerts.find(button) != m_buttonAlerts.end()) {
      m_buttonAlerts[button].Set(false);
    }
  }

  return DriverStation::GetStickButtonPressed(m_port, button);
}

bool GenericHID::GetRawButtonReleased(int button) {
  m_disconnectedAlert.Set(!IsConnected());

  if(button > GetButtonCount()) {
    if(m_buttonAlerts.find(button) == m_buttonAlerts.end()) {
      m_buttonAlerts[button] = Alert {
        "JoystickAlerts/" + std::to_string(m_port),
        "Button " + std::to_string(button) + " does not exist on Joystick " + std::to_string(m_port),
        Alert::AlertType::kWarning
      };

    }
    m_buttonAlerts[button].Set(true);
  } else {
    if(m_buttonAlerts.find(button) != m_buttonAlerts.end()) {
      m_buttonAlerts[button].Set(false);
    }
  }

  return DriverStation::GetStickButtonReleased(m_port, button);
}

BooleanEvent GenericHID::Button(int button, EventLoop* loop) const {
  return BooleanEvent(loop,
                      [this, button]() { return this->GetRawButton(button); });
}

double GenericHID::GetRawAxis(int axis) const {
  m_disconnectedAlert.Set(!IsConnected());

  if(axis > GetAxisCount()) {
    if(m_axisAlerts.find(axis) == m_axisAlerts.end()) {
      m_axisAlerts[axis] = Alert {
        "JoystickAlerts/" + std::to_string(m_port),
        "Axis " + std::to_string(axis) + " does not exist on Joystick " + std::to_string(m_port),
        Alert::AlertType::kWarning
      };

    }
    m_axisAlerts[axis].Set(true);
  } else {
    if(m_axisAlerts.find(axis) != m_axisAlerts.end()) {
      m_axisAlerts[axis].Set(false);
    }
  }

  return DriverStation::GetStickAxis(m_port, axis);
}

int GenericHID::GetPOV(int pov) const {
  m_disconnectedAlert.Set(!IsConnected());

  if(pov > GetPOVCount()) {
    auto result = m_povAlerts.try_emplace(pov, "JoystickAlerts/" + std::to_string(m_port), "POV " + std::to_string(pov) + " does not exist on Joystick " + std::to_string(m_port), Alert::AlertType::kWarning);
    result.first->second.Set(true);
  } else {
    if(m_povAlerts.find(pov) != m_povAlerts.end()) {
      m_povAlerts[pov].Set(false);
    }
  }

  return DriverStation::GetStickPOV(m_port, pov);
}

BooleanEvent GenericHID::POV(int angle, EventLoop* loop) const {
  return POV(0, angle, loop);
}

BooleanEvent GenericHID::POV(int pov, int angle, EventLoop* loop) const {
  return BooleanEvent(
      loop, [this, pov, angle] { return this->GetPOV(pov) == angle; });
}

BooleanEvent GenericHID::POVUp(EventLoop* loop) const {
  return POV(0, loop);
}

BooleanEvent GenericHID::POVUpRight(EventLoop* loop) const {
  return POV(45, loop);
}

BooleanEvent GenericHID::POVRight(EventLoop* loop) const {
  return POV(90, loop);
}

BooleanEvent GenericHID::POVDownRight(EventLoop* loop) const {
  return POV(135, loop);
}

BooleanEvent GenericHID::POVDown(EventLoop* loop) const {
  return POV(180, loop);
}

BooleanEvent GenericHID::POVDownLeft(EventLoop* loop) const {
  return POV(225, loop);
}

BooleanEvent GenericHID::POVLeft(EventLoop* loop) const {
  return POV(270, loop);
}

BooleanEvent GenericHID::POVUpLeft(EventLoop* loop) const {
  return POV(315, loop);
}

BooleanEvent GenericHID::POVCenter(EventLoop* loop) const {
  return POV(360, loop);
}

BooleanEvent GenericHID::AxisLessThan(int axis, double threshold,
                                      EventLoop* loop) const {
  return BooleanEvent(loop, [this, axis, threshold]() {
    return this->GetRawAxis(axis) < threshold;
  });
}

BooleanEvent GenericHID::AxisGreaterThan(int axis, double threshold,
                                         EventLoop* loop) const {
  return BooleanEvent(loop, [this, axis, threshold]() {
    return this->GetRawAxis(axis) > threshold;
  });
}

int GenericHID::GetAxisCount() const {
  return DriverStation::GetStickAxisCount(m_port);
}

int GenericHID::GetPOVCount() const {
  return DriverStation::GetStickPOVCount(m_port);
}

int GenericHID::GetButtonCount() const {
  return DriverStation::GetStickButtonCount(m_port);
}

bool GenericHID::IsConnected() const {
  return DriverStation::IsJoystickConnected(m_port);
}

GenericHID::HIDType GenericHID::GetType() const {
  return static_cast<HIDType>(DriverStation::GetJoystickType(m_port));
}

std::string GenericHID::GetName() const {
  return DriverStation::GetJoystickName(m_port);
}

int GenericHID::GetAxisType(int axis) const {
  return DriverStation::GetJoystickAxisType(m_port, axis);
}

int GenericHID::GetPort() const {
  return m_port;
}

void GenericHID::SetOutput(int outputNumber, bool value) {
  m_outputs =
      (m_outputs & ~(1 << (outputNumber - 1))) | (value << (outputNumber - 1));

  HAL_SetJoystickOutputs(m_port, m_outputs, m_leftRumble, m_rightRumble);
}

void GenericHID::SetOutputs(int value) {
  m_outputs = value;
  HAL_SetJoystickOutputs(m_port, m_outputs, m_leftRumble, m_rightRumble);
}

void GenericHID::SetRumble(RumbleType type, double value) {
  value = std::clamp(value, 0.0, 1.0);
  double rumbleValue = value * 65535;

  if (type == kLeftRumble) {
    m_leftRumble = rumbleValue;
  } else if (type == kRightRumble) {
    m_rightRumble = rumbleValue;
  } else {
    m_leftRumble = rumbleValue;
    m_rightRumble = rumbleValue;
  }

  HAL_SetJoystickOutputs(m_port, m_outputs, m_leftRumble, m_rightRumble);
}
