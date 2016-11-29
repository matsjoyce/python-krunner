import krunner


class Runner(krunner.AbstractRunner):
    def match(self, context):
        m = krunner.QueryMatch(self)
        m.setText("Hello from b")
        m.setIconName("text-x-python")
        context.addMatch(m)
