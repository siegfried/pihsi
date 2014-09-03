FactoryGirl.define do
  factory :speech_recognizer, class: Pihsi::SpeechRecognizer do
    hmm 'spec/fixtures/model/hmm/en_US/hub4wsj_sc_8k'
    lm 'spec/fixtures/model/lm/en/turtle.DMP'
    dict 'spec/fixtures/model/lm/en/turtle.dic'
    pip 1.0
    samprate 16000.0
    maxhmmpf -1

    initialize_with { new(attributes) }
  end
end
