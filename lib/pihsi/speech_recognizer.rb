class Pihsi::SpeechRecognizer
  attr_reader :hmm, :lm, :dict

  def initialize(hmm, lm, dict)
    @hmm, @lm, @dict = hmm, lm, dict
  end

  def recognize(string_buffer)
    'go forward ten meters'
  end
end
