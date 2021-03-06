#include "lightsensor_datacontroler.h"

LightSensorDataControler::LightSensorDataControler(UsbSerial& usb_serial, int size) :
  usb_serial_(usb_serial),
  lightsensor_datapacket_(size) {
  }

LightSensorDataControler::~LightSensorDataControler() {
}

int LightSensorDataControler::SearchBeginPos(char* buffer, const int size) {
  for (int i = 0; i < size - 1; ++i) {
    if (buffer[i] == '*' && buffer[i + 1] == '#')
      return i;
  }
  return -1;
}

bool LightSensorDataControler::CheckDataTail() {
  if (data_[kSerialDataSize - 4] == '#' &&
      data_[kSerialDataSize - 3] == '*' &&
      data_[kSerialDataSize - 2] == '\r' &&
      data_[kSerialDataSize - 1] == '\n')
    return true;
  return false;
}

bool LightSensorDataControler::UpdateData() {
  std::lock_guard<std::mutex> lock(mtx_);
  char buffer[kSerialDataSize];
  usb_serial_.Read(buffer, kSerialDataSize);
  int begin_pos = SearchBeginPos(buffer, kSerialDataSize);
  if (begin_pos == -1) {
    return false;
  }
  else {
    memcpy(data_, buffer + begin_pos, kSerialDataSize - begin_pos);
  }
  usb_serial_.Read(data_ + kSerialDataSize - begin_pos, begin_pos); // read the rest data.

  if (CheckDataTail()) {
    LightSensorDataPacket lsdp;
    memcpy(&lsdp, data_ + 2, sizeof(LightSensorDataPacket));
    if (lightsensor_datapacket_.write(&lsdp, 1) == 1)
      return true;
    else {
      cerr << "ringbuffer is no free" << endl;
      return false;
    }
    return true;
  }
  return false;
}

bool LightSensorDataControler::GetLightSensorDataPacket(LightSensorDataPacket& lsdp) {
  std::lock_guard<std::mutex> lock(mtx_);
  if (lightsensor_datapacket_.read(&lsdp, 1) == 1)
    return true;
  return false;
}
