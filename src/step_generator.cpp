/* Copyright 2013-2015 Matt Tytel
 *
 * mopo is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * mopo is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with mopo.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "step_generator.h"

#include <cmath>

namespace mopo {

  StepGenerator::StepGenerator(int max_steps) :
      Processor(kNumInputs + max_steps, 1), offset_(0.0), current_step_(0) { }

  void StepGenerator::process() {
    unsigned int num_steps = static_cast<int>(inputs_->at(kNumSteps)->at(0));

    int i = 0;
    if (inputs_->at(kReset)->source->triggered &&
        inputs_->at(kReset)->source->trigger_value == kVoiceReset)
      i = inputs_->at(kReset)->source->trigger_offset;

    mopo_float total = 0.0;
    for (; i < buffer_size_; ++i)
      total += inputs_->at(kFrequency)->at(i);

    total /= sample_rate_;
    mopo_float integral;
    offset_ = modf(offset_ + total, &integral);
    current_step_ = (current_step_ + num_steps) % num_steps;

    for (i = 0; i < buffer_size_; ++i)
      outputs_->at(0)->buffer[i] = inputs_->at(kSteps + current_step_)->at(i);
  }
} // namespace mopo
