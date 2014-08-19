FactoryGirl.define do
  factory :speech_recognizer, class: Pihsi::SpeechRecognizer do
    hmm 'spec/fixtures/model/hmm/en_US/hub4wsj_sc_8k'
    lm 'spec/fixture/models/lm/en/turtle.DMP'
    dict 'spec/fixture/models/lm/en/turtle.dic'

    initialize_with { new(hmm, lm, dict) }
  end
end
