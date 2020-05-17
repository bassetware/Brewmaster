

void scale_tick() {
    if(millis() < scale_nextreading) {return;}
    if (scale.is_ready()) {
        scale_ringbuffer[scale_ring_pos] = scale.read();
        long scale_sum = 0;
        for(int i = 0; i < SCALE_RINGCOUNT; i++) {
            scale_sum = scale_sum + scale_ringbuffer[i];
        }

        scale_raw = (scale_sum / SCALE_RINGCOUNT);
        //scale_raw = scale_raw;
        //Serial.println(scale_raw);
        scale_nextreading = millis() + SCALE_INTERVAL;
        scale_ring_pos = ++scale_ring_pos % SCALE_RINGCOUNT;
    }
    else {
        return;
    }
}

void scale_tare_scale() {
    Serial.println("Scale Tare now");
    delay(20);
    long scale_sum = 0;
    for (int i = 0; i < SCALE_RINGCOUNT; i++) {
        scale.wait_ready_retry(20);
        scale_ringbuffer[i] = scale.read();
        scale_sum = scale_sum + scale_ringbuffer[i];
        delay(30);
    }
    scale_tare = int(scale_sum / SCALE_RINGCOUNT);
    scale_raw = 0;
    Serial.println("Tara: ");
    Serial.println(scale_tare);
    scale_nextreading = millis() + SCALE_INTERVAL;
}

float calcMassfromRaw(long raw) {
    float screen_mass = ((float)raw - scale_b) / scale_m;
    return screen_mass;
}