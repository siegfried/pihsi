RSpec.describe Pihsi::SpeechRecognizer do
  let(:instance) { build :speech_recognizer }

  describe '#recognize(string_buffer)' do
    subject { instance.recognize(string_buffer) }

    context 'when string_buffer is the content of goforward.raw' do
      let(:string_buffer) { File.read 'spec/fixtures/goforward.raw' }

      it { should eq "go forward ten meters" }
    end
  end
end
